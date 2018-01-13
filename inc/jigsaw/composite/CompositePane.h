#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
    namespace composite
	{
        class PaneView: public jigsaw::core::View
        {
        public:
            void ApplyVerticalLayout();
            void ApplyHorizontalLayout();
        protected:
            /// \brief 新建部件
            void onSetupUI() override;
        private:
        };

        class PaneComponent : public jigsaw::core::MVCComponent
		{
        public:
            DECLARE_STATIC_NEW(Pane)
        protected:
            DECLARE_CONSTRUCTOR(Pane, MVC)
        };

    }//jigsaw
}//namespace
