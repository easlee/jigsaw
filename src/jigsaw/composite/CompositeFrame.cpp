#include <QWidget>
#include "jigsaw/composite/CompositeFrame.h"

//-----------------------------------------------------------------------
void jigsaw::composite::FrameView::ApplyVerticalLayout()
{
    auto em = this->ToElement<FrameElement>();
    QLayout* oldLayout = em->container->layout();
    QVBoxLayout* newLayout = new QVBoxLayout(em->container);
    newLayout->setSpacing(0);
    newLayout->setMargin(0);

    foreach (View* child, children_)
    {
        if(0 != oldLayout)
            oldLayout->removeWidget(child->element()->root);
        newLayout->addWidget(child->element()->root);
    }

    em->container->setLayout(newLayout);
    if(0 != oldLayout)
    {
        oldLayout->setParent(NULL);
        oldLayout->deleteLater();
    }
}

void jigsaw::composite::FrameView::ApplyHorizontalLayout()
{
    auto em = this->ToElement<FrameElement>();
    QLayout* oldLayout = em->container->layout();
    QHBoxLayout* newLayout = new QHBoxLayout(element()->root);
    newLayout->setSpacing(0);
    newLayout->setMargin(0);

    foreach (View* child, children_)
    {
        if(0 != oldLayout)
            oldLayout->removeWidget(child->element()->root);
        newLayout->addWidget(child->element()->root);
    }

    em->container->setLayout(newLayout);
    if(0 != oldLayout)
    {
        oldLayout->setParent(NULL);
        oldLayout->deleteLater();
    }
}

void jigsaw::composite::FrameView::AlignLeft()
{
    auto em = this->ToElement<FrameElement>();
    em->spaceLeft->setVisible(false);
    em->spaceRight->setVisible(true);
}

void jigsaw::composite::FrameView::AlignRight()
{
    auto em = this->ToElement<FrameElement>();
    em->spaceLeft->setVisible(true);
    em->spaceRight->setVisible(false);
}

void jigsaw::composite::FrameView::AlignHCenter()
{
    auto em = this->ToElement<FrameElement>();
    em->spaceLeft->setVisible(true);
    em->spaceRight->setVisible(true);
}

void jigsaw::composite::FrameView::AlignTop()
{
    auto em = this->ToElement<FrameElement>();
    em->spaceTop->setVisible(false);
    em->spaceBottom->setVisible(true);
}

void jigsaw::composite::FrameView::AlignButtom()
{
    auto em = this->ToElement<FrameElement>();
    em->spaceTop->setVisible(true);
    em->spaceBottom->setVisible(false);
}

void jigsaw::composite::FrameView::AlignVCenter()
{
    auto em = this->ToElement<FrameElement>();
    em->spaceTop->setVisible(true);
    em->spaceBottom->setVisible(true);
}

void jigsaw::composite::FrameView::onSetupUI()
{
    auto em = new FrameElement();
    em->root = View::NewVRoot();
    //element_->root->setBackgroundRole(QPalette::ColorRole::)
    em->spaceTop = new QWidget(em->root);
    em->spaceTop->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    em->root->layout()->addWidget(em->spaceTop);
    QWidget* hFrame = View::NewHRoot();
    hFrame->setParent(em->root);
    em->root->layout()->addWidget(hFrame);
    em->spaceBottom = new QWidget(em->root);
    em->spaceBottom->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    em->root->layout()->addWidget(em->spaceBottom);

    em->spaceLeft = new QWidget(hFrame);
    em->spaceLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hFrame->layout()->addWidget(em->spaceLeft);
    em->container = View::NewRoot();
    em->container->setParent(hFrame);
    hFrame->layout()->addWidget(em->container);
    em->spaceRight = new QWidget(hFrame);
    em->spaceRight->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hFrame->layout()->addWidget(em->spaceRight);

    element_ = em;
}

void jigsaw::composite::FrameView::onAttach(jigsaw::core::View *_child)
{
    assert(_child);
    assert(_child->element());
    assert(_child->element()->root);
    assert(element());
    assert(element()->root);
    auto em = this->ToElement<FrameElement>();
    _child->element()->root->setParent(em->container);
    if(em->container->layout())
        em->container->layout()->addWidget(_child->element()->root);
}

void jigsaw::composite::FrameView::onDetach(jigsaw::core::View *_child)
{
    assert(_child);
    assert(_child->element());
    assert(_child->element()->root);
    assert(element());
    assert(element()->root);
    auto em = this->ToElement<FrameElement>();
    if(em->container->layout())
        em->container->layout()->addWidget(_child->element()->root);
    _child->element()->root->setParent(0);
}
