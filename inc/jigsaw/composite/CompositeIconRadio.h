#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
	namespace composite
	{
        class IconRadioView: public jigsaw::core::View
		{
		public:
            struct IconRadioElement :public jigsaw::core::View::Element
			{
                QRadioButton* ptrRadio;
			};
			
            std::function< void(IconRadioView*) > onClick;
			void SetScaleIcon(const QString& _iconpath);
        protected:
            /// \brief 加载iconradio.ui
			void onSetupUI() override;
            void onReset() override;
		private:
            QString iconStylesheet;
            QString rootStylesheet;
        };

        class IconRadioComponent : public jigsaw::core::MVCComponent
		{
		public:
            DECLARE_STATIC_NEW(IconRadio)
            FORBID_ADDMVCCHILD
		protected:
            DECLARE_CONSTRUCTOR(IconRadio, MVC)
		};
	}
}
