#pragma once

#include "jigsaw/core/Composite.h"

///
/// Usage:
/// \code
/// auto radioComponent = TextRadioComponent::New();
/// auto radioView = radioComponent->ToView<TextRadioView>();
/// radioView->ApplyStyle(ColorPalette::PRIMARY);
/// radioView->ApplySize(TextRadioView::SIZE_NORMAL);
///
namespace jigsaw
{
    namespace composite
    {
        class TextRadioView : public jigsaw::core::View
        {
        public:
            static const QString SIZE_TINY;
            static const QString SIZE_SMALL;
            static const QString SIZE_NORMAL;
            static const QString SIZE_LARGE;
        public:
            struct TextRadioElement :public jigsaw::core::View::Element
            {
                QRadioButton* ptrRadio;
            };

            std::function< void(TextRadioView*) > onClick;
            void SetText(const QString& _text);
            void ApplySize(const QString& _size);
        protected:
            /// \brief 加载textradio.ui并绑定事件
            void onSetupUI() override;
        private:
        };

        class TextRadioComponent : public jigsaw::core::MVCComponent
        {
        public:
            DECLARE_STATIC_NEW(TextRadio)
            FORBID_ADDMVCCHILD
        protected:
            DECLARE_CONSTRUCTOR(TextRadio, MVC)
        };
    }
}
