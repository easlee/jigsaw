#include "jigsaw/composite/CompositeLineInput.h"

void jigsaw::composite::LineInputView::SetFontSize(int _pt)
{
    fontSize_ = _pt;
    updateFont();
}

void jigsaw::composite::LineInputView::SetColor(int _r, int _g, int _b, int _a)
{
    color_ = QColor(_r, _g, _b, _a);
    updateFont();
}

void jigsaw::composite::LineInputView::SetText(const QString &_text)
{
   auto em = this->ToElement<LineInputElement>();
   em->ptrInput->setText(_text);
   curText = _text;
}

void jigsaw::composite::LineInputView::SetBold(bool _value)
{
   bold_ = _value;
   updateFont();
}

void jigsaw::composite::LineInputView::Undo()
{
    auto em = this->ToElement<LineInputElement>();
    em->ptrInput->setText(curText);
}

void jigsaw::composite::LineInputView::onSetupUI()
{
    LineInputElement* element = new LineInputElement();
    element->root = View::NewRoot(":/jigsaw/ui/lineinput");
    element->container = element->root;
    assert(element->root);
    element->ptrInput = element->root->findChild<QLineEdit*>("__input");
    assert(element->ptrInput);
    element_ = element;

    QObject::connect(element->ptrInput, &QLineEdit::returnPressed, [=]{
        if(0 != onSubmit)
            onSubmit(this, element->ptrInput->text());
        curText = element->ptrInput->text();
    });

    QObject::connect(element->ptrInput, &QLineEdit::editingFinished, [=]{
        if(!element->ptrInput->hasFocus())
            element->ptrInput->setText(curText);
    });


    fontSize_ = 10;
    color_ = QColor(0,0,0, 255);
    bold_ = false;
    curText = "";
}

void jigsaw::composite::LineInputView::onReset()
{
    auto em = this->ToElement<LineInputElement>();
    em->ptrInput->setText("");
}

void jigsaw::composite::LineInputView::updateFont()
{
    auto em = this->ToElement<LineInputElement>();
    em->ptrInput->setStyleSheet(QString("QLineEdit#__input{ font: %1 %2pt; color: rgba(%3, %4, %5, %6);}")
                                        .arg(bold_?"bold":"").arg(fontSize_).arg(color_.red()).arg(color_.green()).arg(color_.blue()).arg(color_.alpha()));
}

