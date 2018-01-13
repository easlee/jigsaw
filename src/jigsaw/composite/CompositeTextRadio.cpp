#include "jigsaw/core/ColorPalette.h"
#include "jigsaw/composite/CompositeTextRadio.h"


const QString jigsaw::composite::TextRadioView::SIZE_TINY = "tiny";
const QString jigsaw::composite::TextRadioView::SIZE_SMALL = "small";
const QString jigsaw::composite::TextRadioView::SIZE_NORMAL = "normal";
const QString jigsaw::composite::TextRadioView::SIZE_LARGE = "large";

void jigsaw::composite::TextRadioView::SetText(const QString& _text)
{
    auto em = this->ToElement<TextRadioElement>();
    em->ptrRadio->setText(_text);
}

void jigsaw::composite::TextRadioView::ApplySize(const QString &_size)
{
    auto em = this->ToElement<TextRadioElement>();
    em->root->setProperty("textradiosize", _size);
    em->ptrRadio->setProperty("textradiosize", _size);
}

//-----------------------------------------------------------------------
void jigsaw::composite::TextRadioView::onSetupUI()
{
    auto em = new TextRadioElement;
    em->root = View::NewRoot(":/jigsaw/ui/textradio");
    em->ptrRadio = em->root->findChild<QRadioButton*>("__radio");

    QObject::connect(em->ptrRadio, &QRadioButton::clicked, [&] {
		if (0 != onClick)
			onClick(this);
	});

    element_ = em;

    saveRootStyleSheet();

    QString qss = "";
    auto addQSS = [&](jigsaw::core::StatusColor _statusColor){
        qss.append(QString("QRadioButton[jigsawstyle=\"%1\"]#__radio { background-color: %2; } \
             QRadioButton[jigsawstyle=\"%1\"]#__radio::pressed { background-color: %3; } \
             QRadioButton[jigsawstyle=\"%1\"]#__radio::disabled { background-color: %4; }")
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

