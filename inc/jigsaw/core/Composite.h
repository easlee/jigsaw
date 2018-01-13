#pragma once

#include <assert.h>
#include <functional>
#include <QtWidgets>

namespace jigsaw
{
	namespace core
	{
        class CompositeException
        {
        public:
            CompositeException(QString _reason)
                : reason_(_reason)
            {

            }
            QString reason()
            {
                return reason_;
            }

        private:
            QString reason_;
        };

		class IAction;

        struct Property
		{
            virtual ~Property() {}
		};

        struct Data
        {
            virtual ~Data() {}
        };

		class View
		{
            friend class Model;
            friend class MVCComponent;
		public:
			struct Element
			{
				QWidget* root;
                QWidget* container;
                virtual ~Element() { if(0 != root) { delete root; root = 0; }}
            };

		public:
            virtual ~View();
            static void ApplyTheme(const QString& _theme);
            static QWidget* NewRoot();
            static QWidget* NewHRoot();
            static QWidget* NewVRoot();
            static QWidget* NewRoot(const QString& _ui);

			void ApplyStyle(const QString& _style);

			void Show();
			void Hide();

            View* FindChild(const QString& _uuid);

			template<typename T>
            T* FindChild()
			{
                foreach (View* child, children_)
				{
					T* view = dynamic_cast<T*>(child);
					if (NULL != view)
						return view;
				}
				return NULL;
			}

            template<typename T>
            const T* ToElement() throw(CompositeException)
            {
                if(0 == element_)
                    throw CompositeException("element_ is null");
                T* o = dynamic_cast<T*>(element_);
                if(0 == o)
                    throw CompositeException("ToElement() get a null element");
                return o;
            }

			bool HasTag(const QString& _tag);
            QVariant GetProperty(const QString& _name);

            const Element* element() { return element_; }
			QString componentUUID() { return componentUUID_;  }
		protected:
			View();
            virtual void onSetupUI() = 0;
            /// \brief 将child的父widget设置为root,并在root的布局中加入child
            virtual void onAttach(View* _child);
            /// \brief 将child的父widget设置为空,并在root的布局中移除child
            virtual void onDetach(View* _child);
            virtual void onReset() {}
            virtual void onRefresh(const Data*) {}
            virtual void handleOnApplyTheme(IAction*) {}

            void saveRootStyleSheet();

            /// \brief 将一个标签路由到一个处理函数
            /// \param _handler 处理函数
            void route(const QString& _tag, std::function<void(const Data*)> _handler);

            /// \brief 使用标签和数据进行更新
            /// \param 如果是由component::update(_tag)触发，数据为component::bind(_data)的_data,
            ///        如果是有component::update(_tag, _data)触发，数据为_data
            void onUpdate(const QString& _tag, const Data* _data);

            /// \brief 往component投递一个事件，触发component::onPostEvent
            void postEvent(const QString& _role, const QVariant& _data);

            QString componentUUID_;
			QList<View*> children_;
			Element* element_;
			static QHash<QString, QString> theme_;
            QString rootStyleSheet_;
		private:
            std::function< void(const QString&, const QVariant&) > onPostEvent;
			void addChild(View* _view);
			void removeChild(View* _view);
			void removeChild(const QString& _uuid);
			void addTag(const QString& _tag);
			void removeTag(const QString& _tag);
            void setProperty(const QString& _name, const QVariant& _value);
            QStringList tags_;
            QHash<QString, std::function<void(const Data*)>> handlers;
            QHash<QString, QVariant> properties_;
            static int index_;
        };

        class Model
		{
			friend class MVCComponent;
		public:
            Model();
            void Update(const QString& _tag);
            void Update(const QString& _tag, const Data* _data);
        private:
			void bind(View* _view);
			void unbind(View* _view);
			QList<View*> views_;
            Data* ptrData_;
		};

        /// \note 销毁组件时会自动销毁子组件，如果子组件不想被销毁，可以使用RemoveChild先将子组件移除
		class Component
		{
			friend class MVCComponent;
		public:
			virtual ~Component();
			static Component* Root();

            /// \details 先调用onInitialize,再子组件的Initialize
			void Initialize(Property* _property);
            /// \details 先调用onAwake,再子组件的Awake
            void Awake();
            /// \details 先调用onStartup,再子组件的Startup
            void Startup();
            /// \details 先调用子组件的Release,再调用onRelease
            void Release();

			void AddChild(Component* _component);
			void RemoveChild(Component* _component);
			void RemoveChild(const QString& _uuid);
			QString GetUUID();
            Component* FindChild(const QString& _uuid);
			template<typename T>
            T* FindChild()
			{
                foreach (Component* child, children_.values())
				{
					T* component = dynamic_cast<T*>(child);
					if (NULL != component)
						return component;
				}
				return NULL;
			}
            Component* RecursiveFindChild(const QString& _uuid);
			QList<QString> ListChildren();
			Component* GetParent();

			virtual void AddTag(const QString& _tag);
			virtual void RemovevTag(const QString& _tag);
			virtual bool HasTag(const QString& _tag);
            virtual void SetProperty(const QString& _name, const QVariant& _value);
            QVariant GetProperty(const QString& _name);
        protected:
			Component();
			Component(const QString& _uuid);
            virtual void onInitialize() {}
            virtual void onAwake() {}
            virtual void onStartup() {}
            virtual void onRelease() {}
			QStringList tags_;
            Property* ptrProperty_;
		private:
            Component* ptrParent_;
			QHash<QString, Component*> children_;
            QHash<QString, QVariant> properties_;
			QString uuid_;
			static Component root_;
		};

		class MVCComponent : public Component
		{
		public:
            virtual ~MVCComponent();
            void AddMVCChild(MVCComponent* _component);
			void RemoveMVCChild(MVCComponent* _component);
			void RemoveMVCChild(const QString& _uuid);

            View* GetView();

            template<typename T>
            T* ToView() throw(CompositeException)
            {
                T* o = dynamic_cast<T*>(view_);
                if(0 == o)
                    throw CompositeException("ToView() get a null view");
                return o;
            }

            template<typename T>
            T* ToData() throw(CompositeException)
            {
                T* o = dynamic_cast<T*>(model_->ptrData_);
                if(0 == o)
                    throw CompositeException("ToData() get a null data");
                return o;
            }

            void BindData(Data* _data, bool _recursive = false);
            void UnbindData(bool _recursive = false);

            /// \brief 重置组件到无数据的初始状态
            /// 调用View::onReset
            /// \param _recursive 是否递归调用都有子组件
            void Reset(bool _recursive = false);

            ///
            /// \brief 使用绑定的数据刷新视图
            /// \details
            /// 1. 调用this->onRefresh
            /// 2. 调用view->onRefresh
            /// 3. 调用子组件的Refresh
            void Refresh(bool _recursive = false);

            /// \brief 使用标签更新View层
            /// \details 此函数将触发View::onUpdate
            void Update(const QString& _tag);

            /// \brief 使用标签和数据更新View层
            /// \details 此函数将触发View::onUpdate
            void Update(const QString& _tag, const Data* _data);

			void AddTag(const QString& _tag) override;
			void RemovevTag(const QString& _tag) override;
            void SetProperty(const QString& _name, const QVariant& _value) override;
        protected:
			MVCComponent(View* _view);
			MVCComponent(View* _view, const QString& _uuid);

            /// \brief 路由onPostEvent的事件
            /// \details handler如果返回true,则不将事件传递给父类
            void route(const QString& _role, std::function<bool(const QVariant&)> _handler);

            virtual void onReset() {}

            /// \brief 当Refresh被调用时
            virtual void onRefresh() {}

            /// \brief 当View::postEvent被调用时
            /// \details 先使用handler处理事件，再将事件传递给父类onPostEvent
            virtual void onPostEvent(const QString& _role, const QVariant& _data);

            void addMVCChild(MVCComponent* _component);
            View* view_;
            Model* model_;
		private:
            QHash<QString, std::function<bool(const QVariant&)>> handlers;
        };//class MVCComponent

		class UUIDUtil
		{
		public:
			static QString New();
		};

	} //namespace core
} //namespace jigsaw

#define DECLARE_STATIC_NEW(clazz) \
    static clazz##Component* New() { return new clazz##Component(new clazz##View()); } \
    static clazz##Component* New(const QString& _uuid) { return new clazz##Component(new clazz##View(), _uuid); }

#define DECLARE_CONSTRUCTOR(clazz, base) \
    clazz##Component(jigsaw::core::View* _view) : base##Component(_view) { } \
    clazz##Component(jigsaw::core::View* _view, const QString& _uuid) : base##Component(_view, _uuid) { } \

#define FORBID_ADDMVCCHILD void AddMVCChild(MVCComponent*) { assert(false); }

