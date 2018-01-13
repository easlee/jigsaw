#pragma once

#include "jigsaw/core/Composite.h"

///
/// Usage:
/// \code
/// auto buttonComponent = TextButtonComponent::New();
/// auto buttonView = buttonComponent->ToView<TextButtonView>();
/// buttonView->ApplyStyle(ColorPalette::PRIMARY);
/// buttonView->ApplySize(TextButtonView::SIZE_NORMAL);
///
namespace jigsaw
{
	namespace composite
	{
        class TextButtonView : public jigsaw::core::View
		{
        public:
            static const QString SIZE_TINY;
            static const QString SIZE_SMALL;
            static const QString SIZE_NORMAL;
            static const QString SIZE_LARGE;
        public:
            struct TextButtonElement :public jigsaw::core::View::Element
			{
                QPushButton* ptrButton;
			};
			
            std::function< void(TextButtonView*) > onClick;
            void SetText(const QString& _text);
            void ApplySize(const QString& _size);
		protected:
            /// \brief 加载textbutton.ui并绑定事件
			void onSetupUI() override;
		private:
		};

        class TextButtonComponent : public jigsaw::core::MVCComponent
		{
		public:
            DECLARE_STATIC_NEW(TextButton)
            FORBID_ADDMVCCHILD
		protected:
            DECLARE_CONSTRUCTOR(TextButton, MVC)
		};
	}
}
