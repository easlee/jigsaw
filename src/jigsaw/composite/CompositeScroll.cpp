#include "jigsaw/composite/CompositeScroll.h"

//-----------------------------------------------------------------------
void jigsaw::composite::ScrollView::onSetupUI()
{
    ScrollElement* element = new ScrollElement;
    element->root = View::NewRoot(":/jigsaw/ui/scroll");
    element->container = element->root->findChild<QWidget*>("__slot");
    element->ptrArea= element->root->findChild<QScrollArea*>("__area");
    element_ = element;
}

//-----------------------------------------------------------------------
void jigsaw::composite::ScrollView::onAttach(View* _child)
{
    assert(_child);
    assert(_child->element());
    assert(_child->element()->root);

    if(0 != element()->container->layout())
        element()->container->layout()->addWidget(_child->element()->root);
    _child->element()->root->setParent(element()->container);
}

void jigsaw::composite::ScrollView::onDetach(jigsaw::core::View *_child)
{
    assert(_child);
    assert(_child->element());
    assert(_child->element()->root);

    if(0 != element()->container->layout())
        element()->container->layout()->removeWidget(_child->element()->root);
    _child->element()->root->setParent(0);
}

void jigsaw::composite::VScrollView::onSetupUI()
{
    ScrollView::onSetupUI();
    auto em = this->ToElement<ScrollElement>();
    QLayout* layout = new QVBoxLayout(em->container);
    layout->setMargin(0);
    layout->setSpacing(0);
    em->container->setLayout(layout);
    em->ptrArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void jigsaw::composite::HScrollView::onSetupUI()
{
    ScrollView::onSetupUI();
    auto em = this->ToElement<ScrollElement>();
    QLayout* layout = new QHBoxLayout(em->container);
    layout->setMargin(0);
    layout->setSpacing(0);
    em->container->setLayout(layout);
    em->ptrArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
