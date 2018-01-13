#include "jigsaw/composite/CompositeFollowBar.h"

namespace jigsaw
{
namespace composite
{

class FollowWidget : public QWidget
{
public:
    std::function<void (const QPoint& _pos)> onMove;
    std::function<void ()> onEnter;
    FollowWidget(QWidget* _parent = 0)
        : QWidget(_parent)
        , target(_parent)
    {
        this->setMouseTracking(true);
    }

    void SetTarget(QWidget* _target)
    {
        target = _target;
    }

protected:
    void enterEvent(QEvent* _event)
    {
        if(0 != onEnter)
            onEnter();
    }

    void mouseMoveEvent(QMouseEvent* _event)
    {
        //将鼠标在部件中的位置映射到目标部件中，得到在鼠标在目标部件中的位置
        if(0 != onMove)
            onMove(this->mapTo(target, _event->pos()));
    }
private:
    QWidget* target;
};//class FollowWidget

}//namespace composite
}//namespace jigsaw

void jigsaw::composite::VFollowBarView::onSetupUI()
{
    FollowBarView::FollowBarElement* element = new FollowBarView::FollowBarElement();
    element->root = View::NewRoot(":/jigsaw/ui/vfollowbar");
    assert(element->root);
    element->ptrToolbar = element->root->findChild<QWidget*>("__toolbar");
    assert(element->ptrToolbar);
    element->ptrSlotTools = element->root->findChild<QWidget*>("__slotTools");
    assert(element->ptrSlotTools);
    QWidget* viewport = element->root->findChild<QWidget*>("__viewport");
    assert(viewport);
    element->ptrSpacer = element->root->findChild<QWidget*>("__spacer");
    assert(element->ptrSpacer);

    //
    // ScrollArea
    //    |- viewport
    //        |- followWidget
    //            |- container
    //                |- pane
    //            |- spacer
    FollowWidget* followWidget = new FollowWidget(viewport);
    viewport->layout()->addWidget(followWidget);
    followWidget->SetTarget(viewport->parentWidget());
    followWidget->setLayout(new QVBoxLayout(followWidget));
    followWidget->onMove = [=](const QPoint& _pos) {
        int center = element->ptrSlotTools->height()/2;
        int offset = _pos.y() - center;
        if(offset < 0)
            offset = 0;
        else if(offset > viewport->parentWidget()->height() - element->ptrSlotTools->height())
            offset = viewport->parentWidget()->height() - element->ptrSlotTools->height();

        element->ptrSpacer->setFixedHeight(offset);
    };

    element->container= View::NewVRoot();
    element->container->setParent(followWidget);
    followWidget->layout()->addWidget(element->container);
    element->container->setMouseTracking(true);
    followWidget->layout()->addItem(new QSpacerItem(0,0, QSizePolicy::Preferred, QSizePolicy::Expanding));

    element_ = element;
}

void jigsaw::composite::FollowBarView::onAttach(View* _child)
{
    assert(_child);
    assert(_child->element());
    assert(_child->element()->root);

    auto em = this->ToElement<FollowBarElement>();
    if(_child->HasTag(FollowBarComponent::TAG_TOOL))
    {
        _child->element()->root->setParent(em->ptrSlotTools);
        em->ptrSlotTools->layout()->addWidget(_child->element()->root);
    }
    else
    {
        _child->element()->root->setParent(em->container);
        em->container->layout()->addWidget(_child->element()->root);
    }
    _child->element()->root->setMouseTracking(true);
}

void jigsaw::composite::FollowBarView::onDetach(jigsaw::core::View *_child)
{
    assert(_child);
    assert(_child->element());
    assert(_child->element()->root);

    auto em = this->ToElement<FollowBarElement>();
    if(_child->HasTag(FollowBarComponent::TAG_TOOL))
    {
        em->ptrSlotTools->layout()->removeWidget(_child->element()->root);
        _child->element()->root->setParent(0);
    }
    else
    {
        em->container->layout()->removeWidget(_child->element()->root);
        _child->element()->root->setParent(0);
    }
    _child->element()->root->setMouseTracking(false);
}

const QString jigsaw::composite::FollowBarComponent::TAG_TOOL = "jigsaw.followbar.tool";

//-----------------------------------------------------------------------
jigsaw::composite::FollowBarComponent* jigsaw::composite::FollowBarComponent::NewV()
{
    return new jigsaw::composite::FollowBarComponent(new jigsaw::composite::VFollowBarView());
}

//-----------------------------------------------------------------------
jigsaw::composite::FollowBarComponent* jigsaw::composite::FollowBarComponent::NewV(const QString& _uuid)
{
    return new jigsaw::composite::FollowBarComponent(new jigsaw::composite::VFollowBarView(), _uuid);
}
