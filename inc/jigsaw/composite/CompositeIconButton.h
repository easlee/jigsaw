#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
	namespace composite
	{
		class IconButtonView : public jigsaw::core::View
		{
		public:
			struct IconButtonElement :public jigsaw::core::View::Element
			{
                QPushButton* ptrButton;
			};
			
			std::function< void(IconButtonView*) > onClick;
            void SetIcon(const QString& _iconpath);
            void SetHoverMask(int _r, int _g, int _b, int _a);
            void SetPressedMask(int _r, int _g, int _b, int _a);
        protected:
			void onSetupUI() override;
		private:
            void updateQSS();
            QString qssIcon;
            QString qssHover;
            QString qssPressed;
		};

		class IconButtonComponent : public jigsaw::core::MVCComponent
		{
		public:
            DECLARE_STATIC_NEW(IconButton)
            FORBID_ADDMVCCHILD
		protected:
            DECLARE_CONSTRUCTOR(IconButton, MVC)
		};
	}
}
