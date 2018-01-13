#pragma once

#include "CompositePane.h"

namespace jigsaw
{
    namespace composite
	{
        /// \note Page默认没有布局，需要布局可以调用PaneView::ApplyXXXLayout()
        class PageView: public PaneView
        {
        public:
        protected:
            /// \brief 加载page.ui
            void onSetupUI() override;
        private:
        };

        class PageComponent : public PaneComponent
		{
		public:
            DECLARE_STATIC_NEW(Page)
        protected:
            DECLARE_CONSTRUCTOR(Page, Pane)
        };

    }//jigsaw
}//namespace
