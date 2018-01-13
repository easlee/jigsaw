#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
    namespace composite
	{
        class ScrollView: public jigsaw::core::View
        {
        public:
            struct ScrollElement : public jigsaw::core::View::Element
            {
                QScrollArea* ptrArea;
                QWidget* ptrSlot;
            };
        protected:
            /// \brief 加载scroll.ui
            void onSetupUI() override;
            void onAttach(View* _child) override;
            void onDetach(View* _child) override;
        private:
        };

        class HScrollView : public ScrollView
        {
        protected:
            /// \brief 在ScrollView::onSetupUI后设置水平布局
            void onSetupUI() override;
        };

        class VScrollView : public ScrollView
        {
        protected:
            /// \brief 在ScrollView::onSetupUI后设置垂直布局
            void onSetupUI() override;
        };

        class ScrollComponent : public jigsaw::core::MVCComponent
		{
		public:
            DECLARE_STATIC_NEW(Scroll)
        protected:
            DECLARE_CONSTRUCTOR(Scroll, MVC)
        };

        class HScrollComponent : public ScrollComponent
        {
        public:
            DECLARE_STATIC_NEW(HScroll)
        protected:
            DECLARE_CONSTRUCTOR(HScroll, Scroll)
        };

        class VScrollComponent : public ScrollComponent
        {
        public:
            DECLARE_STATIC_NEW(VScroll)
        protected:
            DECLARE_CONSTRUCTOR(VScroll, Scroll)
        };
    }//jigsaw
}//namespace
