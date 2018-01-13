#include "jigsaw/composite/CompositeImageButton.h"

//-----------------------------------------------------------------------
void jigsaw::composite::ImageButtonView::SetNormalImage(const QString &_image)
{
    qssNormal = QString("QPushButton#__button { border-image: url(%1); }").arg(_image);
    updateQSS();
}

//-----------------------------------------------------------------------
void jigsaw::composite::ImageButtonView::SetHoverImage(const QString &_image)
{
    qssHover = QString("QPushButton#__button::hover { border-image: url(%1); }").arg(_image);
    updateQSS();
}

//-----------------------------------------------------------------------
void jigsaw::composite::ImageButtonView::SetPressedImage(const QString &_image)
{
    qssPressed = QString("QPushButton#__button::pressed { border-image: url(%1); }").arg(_image);
    updateQSS();
}

//-----------------------------------------------------------------------
void jigsaw::composite::ImageButtonView::SetDisabledImage(const QString &_image)
{
    qssDisabled = QString("QPushButton#__button::disabled { border-image: url(%1); }").arg(_image);
    updateQSS();
}

void jigsaw::composite::ImageButtonView::SetNormalImage(const QString &_image, const QRect &_imageBorder, const QRect& _buttonBorder)
{
    qssNormal = QString("QPushButton#__button { border-image: url(%1) %2 %3 %4 %5; border-width: %6 %7 %8 %9; }").arg(_image)
            .arg(_imageBorder.x()).arg(_imageBorder.y()).arg(_imageBorder.width()).arg(_imageBorder.height())
            .arg(_buttonBorder.x()).arg(_buttonBorder.y()).arg(_buttonBorder.width()).arg(_buttonBorder.height());
    updateQSS();
}

void jigsaw::composite::ImageButtonView::SetHoverImage(const QString &_image, const QRect &_imageBorder, const QRect &_buttonBorder)
{
    qssHover = QString("QPushButton#__button::hover { border-image: url(%1) %2 %3 %4 %5; border-width: %6 %7 %8 %9; }").arg(_image)
            .arg(_imageBorder.x()).arg(_imageBorder.y()).arg(_imageBorder.width()).arg(_imageBorder.height())
            .arg(_buttonBorder.x()).arg(_buttonBorder.y()).arg(_buttonBorder.width()).arg(_buttonBorder.height());
    updateQSS();
}

void jigsaw::composite::ImageButtonView::SetPressedImage(const QString &_image, const QRect &_imageBorder, const QRect &_buttonBorder)
{
    qssPressed = QString("QPushButton#__button::pressed { border-image: url(%1) %2 %3 %4 %5; border-width: %6 %7 %8 %9; }").arg(_image)
            .arg(_imageBorder.x()).arg(_imageBorder.y()).arg(_imageBorder.width()).arg(_imageBorder.height())
            .arg(_buttonBorder.x()).arg(_buttonBorder.y()).arg(_buttonBorder.width()).arg(_buttonBorder.height());
    updateQSS();
}

void jigsaw::composite::ImageButtonView::SetDisabledImage(const QString &_image, const QRect &_imageBorder, const QRect &_buttonBorder)
{
    qssDisabled = QString("QPushButton#__button::disabled { border-image: url(%1) %2 %3 %4 %5; border-width: %6 %7 %8 %9; }").arg(_image)
            .arg(_imageBorder.x()).arg(_imageBorder.y()).arg(_imageBorder.width()).arg(_imageBorder.height())
            .arg(_buttonBorder.x()).arg(_buttonBorder.y()).arg(_buttonBorder.width()).arg(_buttonBorder.height());
    updateQSS();
}

//-----------------------------------------------------------------------
void jigsaw::composite::ImageButtonView::onSetupUI()
{
    ImageButtonElement* element = new ImageButtonElement();
    element->root =View::NewHRoot();
    element->container = element->root;

    element->ptrButton = new QPushButton(element->root);
    element->ptrButton->setObjectName("__button");
    element->ptrButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    element->root->layout()->addWidget(element->ptrButton);

    QObject::connect(element->ptrButton, &QPushButton::clicked, [&] {
		if (0 != onClick)
			onClick(this);
	});

    element_ = element;

    saveRootStyleSheet();
}

//-----------------------------------------------------------------------
void jigsaw::composite::ImageButtonView::handleOnApplyTheme(jigsaw::core::IAction* _action)
{
}

void jigsaw::composite::ImageButtonView::updateQSS()
{
    QString qss = QString("%1%2%3%4%5").arg(rootStyleSheet_).arg(qssNormal).arg(qssHover).arg(qssPressed).arg(qssDisabled);
    element()->root->setStyleSheet(qss);
}
