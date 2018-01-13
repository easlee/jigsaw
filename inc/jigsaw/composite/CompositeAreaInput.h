#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
    namespace composite
	{
        class AreaInputView: public jigsaw::core::View
        {
        public:
            struct AreaInputElement : public jigsaw::core::View::Element
            {
                QPlainTextEdit* ptrInput;
            };
            std::function< void(AreaInputView*, const QString& _text) > onSubmit;
            void SetFontSize(int _pt);
            void SetColor(int _r, int _g, int _b, int _a);
            void SetBold(bool _value);

        protected:
            /// \brief 加载areainput.ui
            void onSetupUI() override;
            void onReset() override;
        private:
            void updateFont();
            int fontSize_;
            QColor color_;
            bool bold_;
        };

        class AreaInputComponent : public jigsaw::core::MVCComponent
		{
        public:
            DECLARE_STATIC_NEW(AreaInput)
            FORBID_ADDMVCCHILD
        protected:
            DECLARE_CONSTRUCTOR(AreaInput, MVC)
        };

    }//jigsaw
}//namespace
