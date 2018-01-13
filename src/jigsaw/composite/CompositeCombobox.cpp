#include "jigsaw/composite/CompositeCombobox.h"


void jigsaw::composite::ComboboxView::AddItem(QString _str)
{
    auto em = this->ToElement<ComboboxElement>();
    em->ptrComboxox->addItem(_str);
}

void jigsaw::composite::ComboboxView::AddItems(QStringList _lis)
{
    auto em = this->ToElement<ComboboxElement>();
    em->ptrComboxox->addItems(_lis);
}

void jigsaw::composite::ComboboxView::onSetupUI()
{
    ComboboxElement* element = new ComboboxElement();
    element->root = View::NewRoot(":/jigsaw/ui/combobox");
    element->container = element->root;
    assert(element->root);
    element->ptrComboxox = element->root->findChild<QComboBox*>("__combobox");
    assert(element->ptrComboxox);
    element_ = element;
    element->ptrComboxox->addItem("");

    QObject::connect(element->ptrComboxox, &QComboBox::currentTextChanged, [=](QString _text){
        if (0 != onSelected)
            onSelected(_text);
    });
}

