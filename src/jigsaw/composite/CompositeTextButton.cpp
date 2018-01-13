#include "jigsaw/core/ColorPalette.h"
#include "jigsaw/composite/CompositeTextButton.h"


const QString jigsaw::composite::TextButtonView::SIZE_TINY = "tiny";
const QString jigsaw::composite::TextButtonView::SIZE_SMALL = "small";
const QString jigsaw::composite::TextButtonView::SIZE_NORMAL = "normal";
const QString jigsaw::composite::TextButtonView::SIZE_LARGE = "large";

void jigsaw::composite::TextButtonView::SetText(const QString& _text)
{
    auto em = this->ToElement<TextButtonElement>();
    em->ptrButton->setText(_text);
}

void jigsaw::composite::TextButtonView::ApplySize(const QString &_size)
{
    auto em = this->ToElement<TextButtonElement>();
    em->root->setProperty("textbuttonsize", _size);
    em->ptrButton->setProperty("textbuttonsize", _size);
}

//-----------------------------------------------------------------------
void jigsaw::composite::TextButtonView::onSetupUI()
{
    auto em = new TextButtonElement;
    em->root = View::NewRoot(":/jigsaw/ui/textbutton");
    em->ptrButton = em->root->findChild<QPushButton*>("__button");

    QObject::connect(em->ptrButton, &QPushButton::clicked, [&] {
		if (0 != onClick)
			onClick(this);
	});

    element_ = em;

    saveRootStyleSheet();

    QString qss = "";
    auto addQSS = [&](jigsaw::core::StatusColor _statusColor){
        qss.append(QString("QPushButton[jigsawstyle=\"%1\"]#__button { background-color: %2; } \
             QPushButton[jigsawstyle=\"%1\"]#__button::pressed { background-color: %3; } \
             QPushButton[jigsawstyle=\"%1\"]#__button::disabled { background-color: %4; }")
             .arg(_statusColor.Name())
             .arg(_statusColor.Normal().Hex())
             .arg(_statusColor.Active().Hex())
             .arg(_statusColor.Disabled().Hex())
        );
    };

    addQSS(jigsaw::core::ColorPalette::Primary());
    addQSS(jigsaw::core::ColorPalette::Info());
    addQSS(jigsaw::core::ColorPalette::Success());
    addQSS(jigsaw::core::ColorPalette::Warning());
    addQSS(jigsaw::core::ColorPalette::Danger());

    em->root->setStyleSheet(QString("%1%2").arg(rootStyleSheet_).arg(qss));
    ApplySize(SIZE_NORMAL);
}

