#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
    namespace composite
	{
        class LineInputView: public jigsaw::core::View
        {
        public:
            struct LineInputElement : public jigsaw::core::View::Element
            {
                QLineEdit* ptrInput;
            };
            void SetFontSize(int _pt);
            void SetColor(int _r, int _g, int _b, int _a);
            void SetText(const QString& _text);
            void SetBold(bool _value);
            void Undo();
            std::function< void(LineInputView*, const QString& _text) > onSubmit;
        protected:
            /// \brief 加载lineinput.ui并绑定事件
            void onSetupUI() override;
            void onReset() override;
        private:
            void updateFont();
            int fontSize_;
            QColor color_;
            bool bold_;
            QString curText;
        };

        class LineInputComponent : public jigsaw::core::MVCComponent
		{
        public:
            DECLARE_STATIC_NEW(LineInput)
            FORBID_ADDMVCCHILD
        protected:
            DECLARE_CONSTRUCTOR(LineInput, MVC)
        };

    }//jigsaw
}//namespace
