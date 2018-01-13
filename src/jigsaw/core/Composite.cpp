#include <assert.h>
#include <QUuid>
#include <QtUiTools/QUiLoader>
#include "jigsaw/core/Dispatcher.hxx"
#include "jigsaw/core/Composite.h"

namespace jigsaw
{
	namespace core
	{
		class OnApplyThemeAction : public jigsaw::core::IAction
		{

		};
	}
}

QHash<QString, QString> jigsaw::core::View::theme_;
int jigsaw::core::View::index_ = 0;

//-----------------------------------------------------------------------
void jigsaw::core::View::ApplyTheme(const QString& _theme)
{
	//load all elements
	QDir dir("themes");
	if (!dir.cd(_theme))
		return;

	theme_.clear();
	QStringList entrylist = dir.entryList(QStringList("*.es"));
    foreach (QString element, entrylist)
	{
		QFile file(dir.absoluteFilePath(element));
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			continue;
		QByteArray style = file.readAll();
		theme_.insert(element.remove(".es"), QLatin1String(style));
		file.close();
	}

	NEW_ACTION(jigsaw::core::OnApplyThemeAction, action);
    INVOKE_ACTION(jigsaw::core::OnApplyThemeAction, action);
}

QWidget *jigsaw::core::View::NewRoot()
{
    QWidget* root = new QWidget();
    root->setObjectName(QString("ROOT%1").arg(++index_));
    root->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    return root;
}

QWidget *jigsaw::core::View::NewHRoot()
{
    QWidget* root = NewRoot();
    root->setObjectName(QString("ROOT%1").arg(++index_));
    root->setLayout(new QHBoxLayout(root));
    root->layout()->setSpacing(0);
    root->layout()->setMargin(0);
    return root;
}

QWidget *jigsaw::core::View::NewVRoot()
{
    QWidget* root = NewRoot();
    root->setObjectName(QString("ROOT%1").arg(++index_));
    root->setLayout(new QVBoxLayout(root));
    root->layout()->setSpacing(0);
    root->layout()->setMargin(0);
    return root;
}

QWidget* jigsaw::core::View::NewRoot(const QString& _ui)
{
     QUiLoader loader;
     QFile file(_ui);
     if(!file.open(QFile::ReadOnly))
         return 0;
     QWidget* root = loader.load(&file);
     return root;
}

void jigsaw_core_view_applystyle(QObject* _obj, const QString&_style)
{
    foreach(QObject* obj, _obj->children())
    {
        obj->setProperty("jigsawstyle", _style);
        jigsaw_core_view_applystyle(obj, _style);
    }
}

//-----------------------------------------------------------------------
void jigsaw::core::View::ApplyStyle(const QString& _style)
{
    jigsaw_core_view_applystyle(element()->root, _style);
}

void jigsaw::core::View::Show()
{
	if (NULL == element_)
		return;
	element_->root->setVisible(true);
}

void jigsaw::core::View::Hide()
{
	if (NULL == element_)
		return;
    element_->root->setVisible(false);
}

jigsaw::core::View *jigsaw::core::View::FindChild(const QString &_uuid)
{
    foreach (View* child, children_)
    {
        if(0 == child->componentUUID().compare(_uuid))
            return child;
    }
    return 0;
}

//-----------------------------------------------------------------------
bool jigsaw::core::View::HasTag(const QString& _tag)
{
	return tags_.contains(_tag);
}

void jigsaw::core::View::setProperty(const QString &_name, const QVariant &_value)
{
    properties_[_name] = _value;
}

QVariant jigsaw::core::View::GetProperty(const QString &_name)
{
   if(properties_.contains(_name))
       return properties_.value(_name);
   return QVariant();
}
//-----------------------------------------------------------------------
jigsaw::core::View::View()
	: element_(NULL)
    , rootStyleSheet_("")
{
}

void jigsaw::core::View::onAttach(jigsaw::core::View *_child)
{
    assert(_child);
    assert(_child->element());
    assert(_child->element()->root);
    assert(element());
    assert(element()->root);
    _child->element()->root->setParent(element()->root);
    if(element()->root->layout())
        element()->root->layout()->addWidget(_child->element()->root);
}

void jigsaw::core::View::onDetach(jigsaw::core::View *_child)
{
    assert(_child);
    assert(_child->element());
    assert(_child->element()->root);
    assert(element());
    assert(element()->root);
    if(element()->root->layout())
        element()->root->layout()->removeWidget(_child->element()->root);
    _child->element()->root->setParent(0);
}

void jigsaw::core::View::saveRootStyleSheet()
{
   rootStyleSheet_ = element()->root->styleSheet();
}

//-----------------------------------------------------------------------
jigsaw::core::View::~View()
{
    if(0 != element_)
    {
        delete element_;
        element_ = 0;
    }
}

void jigsaw::core::View::route(const QString &_tag, std::function<void (const Data*)> _handler)
{
    handlers[_tag] = _handler;
}

void jigsaw::core::View::onUpdate(const QString &_tag, const Data* _data)
{
   if(!handlers.contains(_tag))
       return;

   handlers[_tag](_data);
}

void jigsaw::core::View::postEvent(const QString &_role, const QVariant& _data)
{
    if(0 == onPostEvent)
        return;

    onPostEvent(_role, _data);
}

//-----------------------------------------------------------------------
void jigsaw::core::View::addChild(View* _view)
{
	if (children_.contains(_view))
		return;
	children_.append(_view);
}

//-----------------------------------------------------------------------
void jigsaw::core::View::removeChild(const QString& _uuid)
{
	QList<View*> views;
    foreach (View* view, children_)
	{
		if (0 == view->componentUUID_.compare(_uuid))
			views.append(view);
	}

    foreach (View* view, views)
	{
		children_.removeAll(view);
	}
}

//-----------------------------------------------------------------------
void jigsaw::core::View::addTag(const QString& _tag)
{
	if (tags_.contains(_tag))
		return;
	tags_.append(_tag);
}

//-----------------------------------------------------------------------
void jigsaw::core::View::removeTag(const QString& _tag)
{
	if (!tags_.contains(_tag))
		return;
	tags_.removeAll(_tag);
}

//-----------------------------------------------------------------------
void jigsaw::core::View::removeChild(View* _view)
{
	children_.removeAll(_view);
}

void jigsaw::core::Model::bind(View* _view)
{
	if (views_.contains(_view))
		return;
	views_.append(_view);
}

void jigsaw::core::Model::unbind(View* _view)
{
	if (!views_.contains(_view))
		return;
	views_.removeAll(_view);
}

//-----------------------------------------------------------------------
jigsaw::core::Model::Model()
    : ptrData_(0)
{

}

void jigsaw::core::Model::Update(const QString& _tag)
{
    auto itr = views_.cbegin();
    for (; itr != views_.cend(); ++itr)
    {
        (*itr)->onUpdate(_tag, ptrData_);
    }
}

void jigsaw::core::Model::Update(const QString &_tag, const jigsaw::core::Data* _data)
{
    auto itr = views_.cbegin();
    for (; itr != views_.cend(); ++itr)
    {
        (*itr)->onUpdate(_tag, _data);
    }
}

jigsaw::core::Component jigsaw::core::Component::root_;

//-----------------------------------------------------------------------
jigsaw::core::Component::Component()
    : ptrProperty_(0)
    , ptrParent_(0)
{
	uuid_ = QUuid::createUuid().toString().remove("{").remove("}");
}

//-----------------------------------------------------------------------
jigsaw::core::Component::Component(const QString& _uuid)
    : ptrProperty_(0)
    , ptrParent_(0)
{
	uuid_ = _uuid;
}

//-----------------------------------------------------------------------
jigsaw::core::Component::~Component()
{
    auto itr = children_.begin();
    for (; itr != children_.end(); ++itr)
    {
        delete (*itr);
    }
    children_.clear();
}

//-----------------------------------------------------------------------
void jigsaw::core::Component::Initialize(Property* _property)
{
    ptrProperty_ = _property;
	onInitialize();
	auto itr = children_.begin();
	for (; itr != children_.end(); ++itr)
	{
		(*itr)->Initialize(_property);
	}
}

//-----------------------------------------------------------------------
void jigsaw::core::Component::Awake()
{
	onAwake();
	auto itr = children_.begin();
	for (; itr != children_.end(); ++itr)
	{
		(*itr)->Awake();
	}
}

void jigsaw::core::Component::Startup()
{
	onStartup();
	auto itr = children_.begin();
	for (; itr != children_.end(); ++itr)
	{
		(*itr)->Startup();
	}
}

//-----------------------------------------------------------------------
void jigsaw::core::Component::Release()
{
	auto itr = children_.begin();
	for (; itr != children_.end(); ++itr)
	{
		(*itr)->Release();
	}
	onRelease();
}

//-----------------------------------------------------------------------
void jigsaw::core::Component::AddChild(Component* _component)
{
	assert(_component);
	assert(!(_component->GetUUID().isNull() || _component->GetUUID().isEmpty()));

	if (children_.contains(_component->GetUUID()))
		return;
    _component->ptrParent_ = this;
	children_.insert(_component->GetUUID(), _component);
}

//-----------------------------------------------------------------------
void jigsaw::core::Component::RemoveChild(Component* _component)
{
	assert(_component);
	assert(!(_component->GetUUID().isNull() || _component->GetUUID().isEmpty()));

	if (!children_.contains(_component->GetUUID()))
		return;
	children_.remove(_component->GetUUID());
}

//-----------------------------------------------------------------------
void jigsaw::core::Component::RemoveChild(const QString& _uuid)
{
	if (!children_.contains(_uuid))
		return;
	children_.remove(_uuid);
}

//-----------------------------------------------------------------------
QString jigsaw::core::Component::GetUUID()
{
	return uuid_;
}

//-----------------------------------------------------------------------
jigsaw::core::Component* jigsaw::core::Component::FindChild(const QString& _uuid)
{
	if (!children_.contains(_uuid))
		return NULL;
	return children_.value(_uuid);
}

jigsaw::core::Component* jigsaw::core::Component::RecursiveFindChild(const QString& _uuid)
{
	jigsaw::core::Component* component = FindChild(_uuid);
	if (NULL == component)
	{
		auto itr = children_.begin();
		for (; itr != children_.end(); ++itr)
		{
			component = (*itr)->RecursiveFindChild(_uuid);
			if(NULL != component)
				break;
		}
	}
	return component;
}

//-----------------------------------------------------------------------
QList<QString> jigsaw::core::Component::ListChildren()
{
	return children_.keys();
}

//-----------------------------------------------------------------------
jigsaw::core::Component* jigsaw::core::Component::GetParent()
{
    return ptrParent_;
}

//-----------------------------------------------------------------------
void jigsaw::core::Component::AddTag(const QString& _tag)
{
	if (tags_.contains(_tag))
		return;
	tags_.append(_tag);
}

//-----------------------------------------------------------------------
void jigsaw::core::Component::RemovevTag(const QString& _tag)
{
	if (!tags_.contains(_tag))
		return;
	tags_.removeAll(_tag);
}

//-----------------------------------------------------------------------
bool jigsaw::core::Component::HasTag(const QString& _tag)
{
    return tags_.contains(_tag);
}

void jigsaw::core::Component::SetProperty(const QString &_name, const QVariant &_value)
{
    properties_[_name] = _value;
}

QVariant jigsaw::core::Component::GetProperty(const QString &_name)
{
   if(properties_.contains(_name))
       return properties_.value(_name);
   return QVariant();
}

//-----------------------------------------------------------------------
jigsaw::core::Component* jigsaw::core::Component::Root()
{
	return &root_;
}

//-----------------------------------------------------------------------
jigsaw::core::MVCComponent::MVCComponent(View* _view)
	: jigsaw::core::Component()
{
	view_ = _view;
	view_->componentUUID_ = uuid_;
    view_->onPostEvent = std::bind(&MVCComponent::onPostEvent, this, std::placeholders::_1, std::placeholders::_2);
	view_->onSetupUI();
    model_ = new Model();
    model_->bind(view_);
	Dispatcher::Handle<View, jigsaw::core::OnApplyThemeAction>(_view, &View::handleOnApplyTheme);
}

//-----------------------------------------------------------------------
jigsaw::core::MVCComponent::MVCComponent(View* _view, const QString& _uuid)
	: jigsaw::core::Component(_uuid)
{
	view_ = _view;
	view_->componentUUID_ = uuid_;
    view_->onPostEvent = std::bind(&MVCComponent::onPostEvent, this, std::placeholders::_1, std::placeholders::_2);
    view_->onSetupUI();
    model_ = new Model();
    model_->bind(view_);
    Dispatcher::Handle<View, jigsaw::core::OnApplyThemeAction>(_view, &View::handleOnApplyTheme);
}

void jigsaw::core::MVCComponent::route(const QString &_role, std::function<bool(const QVariant &)> _handler)
{
    handlers[_role] = _handler;
}

void jigsaw::core::MVCComponent::onPostEvent(const QString &_role, const QVariant& _data)
{
   bool finish = false;
   if(handlers.contains(_role))
   {
       finish = handlers[_role](_data);
   }

   if(finish)
       return;

   MVCComponent* mvc = dynamic_cast<MVCComponent*>(ptrParent_);
   if(0 != mvc)
   {
       mvc->onPostEvent(_role, _data);
   }
}

//-----------------------------------------------------------------------
jigsaw::core::MVCComponent::~MVCComponent()
{
	if (NULL != view_)
	{
		delete view_;
		view_ = NULL;
	}

    if (NULL != model_)
	{
        delete model_;
        model_ = NULL;
    }
}

void jigsaw::core::MVCComponent::addMVCChild(jigsaw::core::MVCComponent *_component)
{
    this->AddChild(_component);
    view_->addChild(_component->GetView());
    view_->onAttach(_component->GetView());
}

//-----------------------------------------------------------------------
void jigsaw::core::MVCComponent::AddMVCChild(MVCComponent* _component)
{
    addMVCChild(_component);
}
//-----------------------------------------------------------------------
void jigsaw::core::MVCComponent::RemoveMVCChild(MVCComponent* _component)
{
	this->RemoveChild(_component);
	view_->removeChild(_component->GetView());
    view_->onDetach(_component->GetView());
}

void jigsaw::core::MVCComponent::RemoveMVCChild(const QString& _uuid)
{
	this->RemoveChild(_uuid);
    view_->removeChild(_uuid);
}


//-----------------------------------------------------------------------
jigsaw::core::View* jigsaw::core::MVCComponent::GetView()
{
    return view_;
}

void jigsaw::core::MVCComponent::BindData(jigsaw::core::Data *_data, bool _recursive /*= false*/)
{
    model_->ptrData_ = _data;
    if(_recursive)
    {
        foreach (QString uuid, this->ListChildren())
        {
            jigsaw::core::Component* component = this->FindChild(uuid);
            jigsaw::core::MVCComponent* source = dynamic_cast<jigsaw::core::MVCComponent*>(component);
            source->BindData(_data, _recursive);
        }
    }
}

void jigsaw::core::MVCComponent::UnbindData(bool _recursive /*= false*/)
{
   model_->ptrData_ = 0;
   if(_recursive)
   {
       foreach (QString uuid, this->ListChildren())
       {
           jigsaw::core::Component* component = this->FindChild(uuid);
           jigsaw::core::MVCComponent* source = dynamic_cast<jigsaw::core::MVCComponent*>(component);
           source->UnbindData(_recursive);
       }
   }
}

//-----------------------------------------------------------------------
void jigsaw::core::MVCComponent::Reset(bool _recursive /* = false */)
{
    onReset();
	view_->onReset();
	if (_recursive)
	{
        foreach (QString uuid, this->ListChildren())
		{
			jigsaw::core::Component* component = this->FindChild(uuid);
			jigsaw::core::MVCComponent* source = dynamic_cast<jigsaw::core::MVCComponent*>(component);
            source->Reset(_recursive);
		}
    }
}

void jigsaw::core::MVCComponent::Refresh(bool _recursive)
{
    onRefresh();
    view_->onRefresh(model_->ptrData_);
    if (_recursive)
    {
        foreach (QString uuid, this->ListChildren())
        {
            jigsaw::core::Component* component = this->FindChild(uuid);
            jigsaw::core::MVCComponent* source = dynamic_cast<jigsaw::core::MVCComponent*>(component);
            source->Refresh(_recursive);
        }
    }
}

void jigsaw::core::MVCComponent::Update(const QString& _tag)
{
    model_->Update(_tag);
}

void jigsaw::core::MVCComponent::Update(const QString &_tag, const jigsaw::core::Data* _data)
{
    model_->Update(_tag, _data);
}

void jigsaw::core::MVCComponent::AddTag(const QString& _tag)
{
	Component::AddTag(_tag);
	view_->addTag(_tag);
}

void jigsaw::core::MVCComponent::RemovevTag(const QString& _tag)
{
	Component::RemoveChild(_tag);
    view_->removeTag(_tag);
}

void jigsaw::core::MVCComponent::SetProperty(const QString &_name, const QVariant &_value)
{
    Component::SetProperty(_name, _value);
    view_->setProperty(_name,_value);
}


QString jigsaw::core::UUIDUtil::New()
{
	return QUuid::createUuid().toString().remove("{").remove("}");
}
