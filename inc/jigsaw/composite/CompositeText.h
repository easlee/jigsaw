#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
    namespace composite
	{
        class TextView: public jigsaw::core::View
        {
        public:
            struct TextElement : public jigsaw::core::View::Element
            {
                QLabel* ptrText;
            };
            void SetFontSize(int _pt);
            void SetText(const QString& _text);
            void SetColor(int _r, int _g, int _b, int _a);
            void SetBold(bool _value);

        protected:
            /// \brief 加载text.ui
            void onSetupUI() override;
        private:
            void updateFont();
            int fontSize_;
            QColor color_;
            bool bold_;
        };

        class TextComponent : public jigsaw::core::MVCComponent
		{
        public:
            DECLARE_STATIC_NEW(Text)
            FORBID_ADDMVCCHILD
        protected:
            DECLARE_CONSTRUCTOR(Text, MVC)
        };

    }//jigsaw
}//namespace
