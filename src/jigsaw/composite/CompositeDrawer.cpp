#include "jigsaw/composite/CompositeDrawer.h"

namespace jigsaw
{
namespace composite
{
namespace DrawerTag
{
    const QString TAG_TAB = "drawer:tab";
    const QString TAG_SECTION = "drawer:section";
    const QString TAG_HEADER = "drawer:header";
    const QString TAG_FOOTER = "drawer:footer";
}//namespace DrawerTag

class SectionView : public core::View
{
public:
    struct SectionElement : core::View::Element
    {
        QLabel* ptrLabel;
    };

    void onSetupUI() override
    {
        SectionElement* element = new SectionElement();
        element->root = core::View::NewHRoot();
        element->ptrLabel = new QLabel(element->root);
        element->ptrLabel->setAccessibleName(DrawerTag::TAG_SECTION);
        element->root->layout()->addWidget(element->ptrLabel);

        element_ = element;
    }
};

class SectionComponent : public core::MVCComponent
{
public:
    SectionComponent(core::View* _view)
        : MVCComponent(_view)
    {

    }

    SectionComponent(core::View* _view, const QString& _uuid)
        : MVCComponent(_view, _uuid)
    {

    }
};

class TabView : public core::View
{
public:
    struct TabElement : core::View::Element
    {
        QRadioButton* ptrButton;
    };

    void onSetupUI() override
    {
        TabElement* element = new TabElement();
        element->root = core::View::NewHRoot();
        element->ptrButton = new QRadioButton(element->root);
        element->root->layout()->addWidget(element->ptrButton);

        element_ = element;
    }
};

class TabComponent : public core::MVCComponent
{
public:
    TabComponent(core::View* _view)
        : MVCComponent(_view)
    {

    }

    TabComponent(core::View* _view, const QString& _uuid)
        : MVCComponent(_view, _uuid)
    {

    }
};

}//namespace composite
}//namespace jigsaw


//-----------------------------------------------------------------------
void jigsaw::composite::DrawerView::ClickTab(const QString &_tabUUID)
{
    auto tabView = dynamic_cast<TabView*>(this->FindChild(_tabUUID));
    assert(tabView);
    auto tabEM = tabView->ToElement<TabView::TabElement>();
    tabEM->ptrButton->click();
}

void jigsaw::composite::DrawerView::onSetupUI()
{
    DrawerElement* element = new DrawerElement();
    element->root = core::View::NewRoot(":/jigsaw/ui/drawer");
    assert(element->root);
    element->container= element->root->findChild<QWidget*>("__container");
    element->ptrSlotHeader = element->root->findChild<QWidget*>("__slotHeader");
    element->ptrSlotFooter = element->root->findChild<QWidget*>("__slotFooter");
    element->group = new QButtonGroup(element->root);
    // 隐藏预览效果时使用的模板
    QLabel* section = element->root->findChild<QLabel*>("__templateSection");
    assert(section);
    section->setVisible(false);
    QRadioButton* tab = element->root->findChild<QRadioButton*>("__templateTab");
    assert(tab);
    tab->setVisible(false);

    element_ = element;
}

//-----------------------------------------------------------------------
void jigsaw::composite::DrawerView::onAttach(View* _child)
{
    assert(_child);
    assert(_child->element());
    assert(_child->element()->root);

    auto em = this->ToElement<DrawerElement>();

    if(_child->HasTag(DrawerTag::TAG_HEADER))
    {
        _child->element()->root->setParent(em->ptrSlotHeader);
        em->ptrSlotHeader->layout()->addWidget(_child->element()->root);
    }
    else if(_child->HasTag(DrawerTag::TAG_FOOTER))
    {
        _child->element()->root->setParent(em->ptrSlotFooter);
        em->ptrSlotFooter->layout()->addWidget(_child->element()->root);
    }
    else
    {
        _child->element()->root->setParent(em->container);
        em->container->layout()->addWidget(_child->element()->root);
        if(_child->HasTag(DrawerTag::TAG_TAB))
        {
            auto tabElement = _child->ToElement<TabView::TabElement>();
            em->group->addButton(tabElement->ptrButton);
        }
    }

}

void jigsaw::composite::DrawerView::onDetach(jigsaw::core::View *_child)
{
    assert(_child);
    assert(_child->element());
    assert(_child->element()->root);

    auto em = this->ToElement<DrawerElement>();
    if(_child->HasTag(DrawerTag::TAG_HEADER))
    {
        em->ptrSlotHeader->layout()->removeWidget(_child->element()->root);
        _child->element()->root->setParent(0);
    }
    else if(_child->HasTag(DrawerTag::TAG_FOOTER))
    {
        em->ptrSlotFooter->layout()->removeWidget(_child->element()->root);
        _child->element()->root->setParent(0);
    }
    else
    {
        em->container->layout()->removeWidget(_child->element()->root);
        _child->element()->root->setParent(0);
        if(_child->HasTag(DrawerTag::TAG_TAB))
        {
            auto tabElement = _child->ToElement<TabView::TabElement>();
            em->group->removeButton(tabElement->ptrButton);
        }
    }
}

//-----------------------------------------------------------------------
void jigsaw::composite::DrawerComponent::NewSection(const QString& _uuid, const QString& _text)
{
    SectionComponent* sectionComponent = new SectionComponent(new SectionView(), _uuid);
    sectionComponent->AddTag(DrawerTag::TAG_SECTION);
    this->AddMVCChild(sectionComponent);
    sectionComponent->Initialize(0);
    auto sectionElement = sectionComponent->GetView()->ToElement<SectionView::SectionElement>();
    sectionElement->ptrLabel->setAccessibleName(DrawerTag::TAG_SECTION);
    sectionElement->ptrLabel->setText(_text);
}

//-----------------------------------------------------------------------
QString jigsaw::composite::DrawerComponent::NewSection(const QString& _text)
{
    QString uuid = jigsaw::core::UUIDUtil::New();
    NewSection(uuid, _text);
    return uuid;
}

//-----------------------------------------------------------------------
void jigsaw::composite::DrawerComponent::NewTab(const QString& _uuid, const QString& _text)
{
    TabComponent* tabComponent = new TabComponent(new TabView(), _uuid);
    tabComponent->AddTag(DrawerTag::TAG_TAB);
    this->AddMVCChild(tabComponent);
    tabComponent->Initialize(0);
    auto tabElement = tabComponent->GetView()->ToElement<TabView::TabElement>();
    tabElement->ptrButton->setAccessibleName(DrawerTag::TAG_TAB);
    tabElement->ptrButton->setText(_text);

    auto view = this->ToView<DrawerView>();
    QObject::connect(tabElement->ptrButton, &QRadioButton::clicked, [=](){
        if(0 != view->onTabClick)
            view->onTabClick(tabComponent->GetUUID());
    });
}

//-----------------------------------------------------------------------
QString jigsaw::composite::DrawerComponent::NewTab(const QString& _text)
{
	QString tabUUID = jigsaw::core::UUIDUtil::New();
	NewTab(tabUUID, _text);
	return tabUUID;
}

//-----------------------------------------------------------------------
void jigsaw::composite::DrawerComponent::SetTabIcon(const QString& _tabUUID, const QString& _iconPath, const QSize& _iconSize)
{
    Component* component = this->FindChild(_tabUUID);
    if(0 == component)
        return;

    auto tabComponent = dynamic_cast<TabComponent*>(component);
    if(0 == tabComponent)
        return;

    auto tabElement = tabComponent->GetView()->ToElement<TabView::TabElement>();
    if(0 == tabElement)
        return;
    tabElement->ptrButton->setIconSize(_iconSize);
    tabElement->ptrButton->setIcon(QPixmap(_iconPath));
}

void jigsaw::composite::DrawerComponent::AttachHeader(jigsaw::core::MVCComponent *_component)
{
    _component->AddTag(DrawerTag::TAG_HEADER);
    this->addMVCChild(_component);
}

void jigsaw::composite::DrawerComponent::AttachFooter(jigsaw::core::MVCComponent *_component)
{
    _component->AddTag(DrawerTag::TAG_FOOTER);
    this->addMVCChild(_component);
}
