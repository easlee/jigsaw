#include "jigsaw/core/ColorPalette.h"
#include "jigsaw/composite/CompositeFlow.h"
#include "jigsaw/composite/CompositePalette.h"

namespace jigsaw
{
namespace composite
{

namespace PaletteTag
{
    const QString REPAINT = "palette.repaint";
}//namespace PaletteTag

struct PaletteData : public jigsaw::core::Data
{
    QString stylesheet;
};

class PaletteButtonView : public jigsaw::core::View
{
public:
    struct PaletteButtonElement: jigsaw::core::View::Element
    {
        QRadioButton* ptrRadio;
    };

protected:
    void onSetupUI() override
    {
        auto em = new PaletteButtonElement();
        em->root = View::NewHRoot();
        em->container = em->root;
        em->ptrRadio = new QRadioButton(em->root);
        em->root->layout()->addWidget(em->ptrRadio);
        element_ = em;
    }
};

class PaletteButtonComponent : public jigsaw::core::MVCComponent
{
public:
    DECLARE_STATIC_NEW(PaletteButton)
protected:
    DECLARE_CONSTRUCTOR(PaletteButton, MVC)
};

class PaletteComponentInternal
{
public:
    void AddColor(jigsaw::core::Color _color)
    {
        auto buttonComponent = PaletteButtonComponent::New();
        buttonComponent->Initialize(ptrExternal->ptrProperty_);
        flowComponent->AddMVCChild(buttonComponent);

        auto buttonElement = buttonComponent->GetView()->ToElement<PaletteButtonView::PaletteButtonElement>();
        ptrButtonGroup->addButton(buttonElement->ptrRadio);
        buttonElement->ptrRadio->setProperty("color_fullname", _color.FullName());
        buttonElement->ptrRadio->setProperty("color_hex", _color.Hex());

        QObject::connect(buttonElement->ptrRadio, &QRadioButton::clicked, [=](){
           auto view = ptrExternal->ToView<PaletteView>();
           if(0 != view->onPick)
               view->onPick(buttonElement->ptrRadio->property("color_hex").toString());
           });
        qss = qss.append(QString("QRadioButton[color_fullname=\"%1\"]{background-color: %2}").arg(_color.FullName()).arg(_color.Hex()));
        buttons.append(buttonComponent);
    }

public:
    PaletteComponent* ptrExternal;
    FlowComponent* flowComponent;
    QList<PaletteButtonComponent*> buttons;
    QString qss;
    QButtonGroup* ptrButtonGroup;
};

}//namespace composite
}//namespace jigsaw

void jigsaw::composite::PaletteView::onSetupUI()
{
    element_ = new View::Element();
    element_->root = View::NewRoot(":/jigsaw/ui/palette");
    element_->container = element_->root;
    assert(element_->root->layout());

    saveRootStyleSheet();

    route(PaletteTag::REPAINT, std::bind(&PaletteView::handleRepaint, this, std::placeholders::_1));
}

void jigsaw::composite::PaletteView::handleRepaint(const jigsaw::core::Data *_data)
{
    auto data = dynamic_cast<const PaletteData*>(_data);
    element()->root->setStyleSheet(QString("%1%2").arg(rootStyleSheet_).arg(data->stylesheet));
}

void jigsaw::composite::PaletteComponent::onInitialize()
{
    internal_ = new PaletteComponentInternal();
    internal_->ptrExternal = this;
    internal_->flowComponent = FlowComponent::New();
    internal_->ptrButtonGroup = new QButtonGroup(internal_->flowComponent->GetView()->element()->root);
    this->addMVCChild(internal_->flowComponent);
}

void jigsaw::composite::PaletteComponent::onStartup()
{
    internal_->AddColor(jigsaw::core::ColorPalette::Primary().Normal());
    internal_->AddColor(jigsaw::core::ColorPalette::Primary().Active());
    internal_->AddColor(jigsaw::core::ColorPalette::Primary().Disabled());
    internal_->AddColor(jigsaw::core::ColorPalette::Info().Normal());
    internal_->AddColor(jigsaw::core::ColorPalette::Info().Active());
    internal_->AddColor(jigsaw::core::ColorPalette::Info().Disabled());
    internal_->AddColor(jigsaw::core::ColorPalette::Success().Normal());
    internal_->AddColor(jigsaw::core::ColorPalette::Success().Active());
    internal_->AddColor(jigsaw::core::ColorPalette::Success().Disabled());
    internal_->AddColor(jigsaw::core::ColorPalette::Warning().Normal());
    internal_->AddColor(jigsaw::core::ColorPalette::Warning().Active());
    internal_->AddColor(jigsaw::core::ColorPalette::Warning().Disabled());
    internal_->AddColor(jigsaw::core::ColorPalette::Danger().Normal());
    internal_->AddColor(jigsaw::core::ColorPalette::Danger().Active());
    internal_->AddColor(jigsaw::core::ColorPalette::Danger().Disabled());
    internal_->AddColor(jigsaw::core::ColorPalette::White());
    internal_->AddColor(jigsaw::core::ColorPalette::Gray());
    internal_->AddColor(jigsaw::core::ColorPalette::Black());

    PaletteData data;
    data.stylesheet = internal_->qss;
    this->Update(PaletteTag::REPAINT, &data);
}

void jigsaw::composite::PaletteComponent::onRelease()
{
    if(0 != internal_)
    {
        if(0 != internal_->flowComponent)
        {
            this->RemoveMVCChild(internal_->flowComponent);
            delete internal_->flowComponent;
            internal_->flowComponent = 0;
        }
        delete internal_;
        internal_ = 0;
    }
}
