#include "jigsaw/composite/CompositeText.h"
void jigsaw::composite::TextView::SetFontSize(int _pt)
{
    fontSize_ = _pt;
    updateFont();
}

void jigsaw::composite::TextView::SetText(const QString &_text)
{
    auto em = this->ToElement<TextElement>();
    em->ptrText->setText(_text);
}

void jigsaw::composite::TextView::SetColor(int _r, int _g, int _b, int _a)
{
    color_ = QColor(_r, _g, _b, _a);
    updateFont();
}

void jigsaw::composite::TextView::SetBold(bool _value)
{
   bold_ = _value;
   updateFont();
}

void jigsaw::composite::TextView::onSetupUI()
{
    TextElement* element = new TextElement();
    element->root = View::NewRoot(":/jigsaw/ui/text");
    element->container = element->root;
    assert(element->root);
    element->ptrText = element->root->findChild<QLabel*>("__text");
    assert(element->ptrText);
    element_ = element;

    fontSize_ = 10;
    color_ = QColor(0,0,0, 255);
    bold_ = false;
}

void jigsaw::composite::TextView::updateFont()
{
    auto em = this->ToElement<TextElement>();
    em->ptrText->setStyleSheet(QString("QLabel#__text{ font: %1 %2pt; color: rgba(%3, %4, %5, %6);}")
                                        .arg(bold_?"bold":"").arg(fontSize_).arg(color_.red()).arg(color_.green()).arg(color_.blue()).arg(color_.alpha()));
}
