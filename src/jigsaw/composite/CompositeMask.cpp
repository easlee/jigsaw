#include "jigsaw/composite/CompositeMask.h"

void jigsaw::composite::MaskView::onSetupUI()
{
    element_ = new View::Element();
    element_->root = View::NewRoot();
    QDialog* dialog = new QDialog();
    dialog->setLayout(new QHBoxLayout(dialog));
    dialog->layout()->setMargin(0);
    dialog->layout()->setSpacing(0);
    dialog->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    dialog->setAttribute(Qt::WA_TranslucentBackground);
    element_->container = dialog;
}

void jigsaw::composite::MaskView::onAttach(jigsaw::core::View *_child)
{
    _child->element()->root->setParent(element()->container);
    element()->container->layout()->addWidget(_child->element()->root);
}

void jigsaw::composite::MaskView::onDetach(jigsaw::core::View *_child)
{
    element()->container->layout()->removeWidget(_child->element()->root);
    _child->element()->root->setParent(0);
}

void jigsaw::composite::MaskView::Popup()
{
    QWidget* parent = element()->root->parentWidget();
    assert(parent);
    QPoint pos = parent->mapToGlobal(QPoint(0, 0));
    element()->container->setGeometry(QRect(pos.x(), pos.y(), parent->width(), parent->height()));
    auto dialog = dynamic_cast<QDialog*>(element()->container);
    dialog->exec();
}

void jigsaw::composite::MaskView::Close()
{
    auto dialog = dynamic_cast<QDialog*>(element()->container);
    dialog->accept();
}
