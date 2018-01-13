#include "jigsaw/composite/CompositePane.h"

//-----------------------------------------------------------------------
void jigsaw::composite::PaneView::ApplyVerticalLayout()
{
    QLayout* oldLayout = element_->root->layout();
    QVBoxLayout* newLayout = new QVBoxLayout(element()->root);
    newLayout->setSpacing(0);
    newLayout->setMargin(0);

    foreach (View* child, children_)
    {
        if(0 != oldLayout)
            oldLayout->removeWidget(child->element()->root);
        newLayout->addWidget(child->element()->root);
    }

    element_->root->setLayout(newLayout);
    if(0 != oldLayout)
    {
        oldLayout->setParent(NULL);
        oldLayout->deleteLater();
    }
}

void jigsaw::composite::PaneView::ApplyHorizontalLayout()
{
    QLayout* oldLayout = element_->root->layout();
    QHBoxLayout* newLayout = new QHBoxLayout(element()->root);
    newLayout->setSpacing(0);
    newLayout->setMargin(0);

    foreach (View* child, children_)
    {
        if(0 != oldLayout)
            oldLayout->removeWidget(child->element()->root);
        newLayout->addWidget(child->element()->root);
    }

    element_->root->setLayout(newLayout);
    if(0 != oldLayout)
    {
        oldLayout->setParent(NULL);
        oldLayout->deleteLater();
    }
}

void jigsaw::composite::PaneView::onSetupUI()
{
    element_ = new View::Element();
    element_->root = View::NewRoot();
    //element_->root->setBackgroundRole(QPalette::ColorRole::)
    element_->container = element_->root;
}
