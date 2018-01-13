#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
	namespace composite
	{
        class DrawerView : public jigsaw::core::View
		{
		public:
            struct DrawerElement : public jigsaw::core::View::Element
			{
                QWidget* ptrSlotHeader;
                QWidget* ptrSlotFooter;
                QButtonGroup* group;
			};
            std::function<void(const QString& _tabUUID)> onTabClick;
            void ClickTab(const QString& _tabUUID);
        protected:
            /// \brief 加载drawer.ui
			void onSetupUI() override;
			void onAttach(View* _child) override;
            void onDetach(View* _child) override;
		private:
			
		};

        class DrawerComponent : public jigsaw::core::MVCComponent
		{
        public:
            DECLARE_STATIC_NEW(Drawer)

            /// \note 在Awake和Startup时调用
            void NewSection(const QString& _uuid, const QString& _text);
            /// \note 在Awake和Startup时调用
            QString NewSection(const QString& _text);

            /// \note 在Awake和Startup时调用
            void NewTab(const QString& _uuid, const QString& _text);
            /// \note 在Awake和Startup时调用
            QString NewTab(const QString& _text);

            void SetTabIcon(const QString& _tabUUID, const QString& _iconPath, const QSize& _iconSize);

            void AttachHeader(MVCComponent* _component);
            void AttachFooter(MVCComponent* _component);
        protected:
            DECLARE_CONSTRUCTOR(Drawer, MVC)
		private:
		};
	}
}
