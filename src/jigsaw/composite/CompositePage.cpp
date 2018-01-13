#include "jigsaw/composite/CompositePage.h"

//-----------------------------------------------------------------------
void jigsaw::composite::PageView::onSetupUI()
{
    element_ = new View::Element();
    element_->root = View::NewRoot(":/jigsaw/ui/page");
    element_->container = element_->root;
    element_->root->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
