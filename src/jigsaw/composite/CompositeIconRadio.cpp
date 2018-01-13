#include "jigsaw/composite/CompositeIconRadio.h"

void jigsaw::composite::IconRadioView::SetScaleIcon(const QString& _iconpath)
{
    iconStylesheet = QString("QRadioButton#__radio{border-image: url(%1);}").arg(_iconpath);
    auto em = this->ToElement<IconRadioElement>();
    em->ptrRadio->setStyleSheet(rootStylesheet.append(iconStylesheet));
}

//-----------------------------------------------------------------------
void jigsaw::composite::IconRadioView::onSetupUI()
{
    IconRadioElement* element = new IconRadioElement();
    element->root = View::NewRoot(":/jigsaw/ui/iconradio");
    element->container = element->root;
    assert(element->root);
    element->ptrRadio = element->root->findChild<QRadioButton*>("__radio");
    assert(element->ptrRadio);
    rootStylesheet = element->root->styleSheet();
    element_ = element;

    QObject::connect(element->ptrRadio, &QRadioButton::clicked, [=](){
       if(0 != onClick)
           onClick(this);
    });

    QObject::connect(element->ptrRadio, &QRadioButton::toggled,[=](bool _toggled){
        QString styleSheet = rootStylesheet;
         if(_toggled)
         {
             //选中时，合并原始样式表和图标样式表，有图有边框
             styleSheet = styleSheet.append(iconStylesheet);
         }
         else
         {
             //非选中时，合并原始样式表、图标样式表、透明背景样式表，有图无边框
             styleSheet = styleSheet.append(iconStylesheet).append("QWidget#__root{background:transparent;}");
         }
         this->element()->root->setStyleSheet(styleSheet);
    });

    emit element->ptrRadio->toggled(false);

}

void jigsaw::composite::IconRadioView::onReset()
{
    auto em = this->ToElement<IconRadioElement>();
    emit em->ptrRadio->toggled(false);
}
