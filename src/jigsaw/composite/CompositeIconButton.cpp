#include "jigsaw/composite/CompositeIconButton.h"

void jigsaw::composite::IconButtonView::SetIcon(const QString& _iconpath)
{
    qssIcon = QString("QWidget#%1{background-image: url(%2);}").arg(element()->root->objectName()).arg(_iconpath);
    updateQSS();
}

//-----------------------------------------------------------------------
void jigsaw::composite::IconButtonView::SetHoverMask(int _r, int _g, int _b, int _a)
{
    qssHover = QString("QPushButton#__button::hover{background-color:rgba(%1, %2, %3, %4);}").arg(_r).arg(_g).arg(_b).arg(_a);
    updateQSS();
}

void jigsaw::composite::IconButtonView::SetPressedMask(int _r, int _g, int _b, int _a)
{
    qssPressed = QString("QPushButton#__button::pressed{background-color:rgba(%1, %2, %3, %4);}").arg(_r).arg(_g).arg(_b).arg(_a);
    updateQSS();
}

void jigsaw::composite::IconButtonView::onSetupUI()
{
    auto em = new IconButtonElement();
    em->root = View::NewHRoot();
    em->container = em->root;
    em->ptrButton = new QPushButton(em->root);
    em->root->layout()->addWidget(em->ptrButton);
    em->ptrButton->setObjectName("__button");
    em->ptrButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QObject::connect(em->ptrButton, &QPushButton::clicked, [&] {
		if (0 != onClick)
			onClick(this);
	});

    element_ = em;
    em->root->setStyleSheet("QPushButton#__button{background-color:transparent;}");
    saveRootStyleSheet();
}

void jigsaw::composite::IconButtonView::updateQSS()
{
    QString qss = QString("%1%2%3%4").arg(rootStyleSheet_).arg(qssIcon).arg(qssHover).arg(qssPressed);
    element()->root->setStyleSheet(qss);
}
