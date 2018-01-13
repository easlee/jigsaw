#include "jigsaw/composite/CompositeAreaInput.h"
void jigsaw::composite::AreaInputView::SetFontSize(int _pt)
{
    fontSize_ = _pt;
    updateFont();
}


void jigsaw::composite::AreaInputView::SetColor(int _r, int _g, int _b, int _a)
{
    color_ = QColor(_r, _g, _b, _a);
    updateFont();
}

void jigsaw::composite::AreaInputView::SetBold(bool _value)
{
   bold_ = _value;
   updateFont();
}

void jigsaw::composite::AreaInputView::onSetupUI()
{
    AreaInputElement* element = new AreaInputElement();
    element->root = View::NewRoot(":/jigsaw/ui/areainput");
    element->container = element->root;
    assert(element->root);
    element->ptrInput = element->root->findChild<QPlainTextEdit*>("__input");
    assert(element->ptrInput);
    element_ = element;

    QObject::connect(element->ptrInput, &QPlainTextEdit::textChanged, [=]{
        if(0 != onSubmit)
            onSubmit(this, element->ptrInput->toPlainText());
    });

    fontSize_ = 10;
    color_ = QColor(0,0,0, 255);
    bold_ = false;
}

void jigsaw::composite::AreaInputView::onReset()
{
    auto em = this->ToElement<AreaInputElement>();
    em->ptrInput->setPlainText("");
}

void jigsaw::composite::AreaInputView::updateFont()
{
    auto element = this->ToElement<AreaInputElement>();
    element->ptrInput->setStyleSheet(QString("QPlainTextEdit#__input{ font: %1 %2pt; color: rgba(%3, %4, %5, %6);}")
                                        .arg(bold_?"bold":"").arg(fontSize_).arg(color_.red()).arg(color_.green()).arg(color_.blue()).arg(color_.alpha()));
}
