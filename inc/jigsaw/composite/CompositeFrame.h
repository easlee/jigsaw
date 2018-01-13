#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
    namespace composite
	{
        class FrameView: public jigsaw::core::View
        {
        public:
            struct FrameElement : public jigsaw::core::View::Element
            {
                QWidget* spaceTop;
                QWidget* spaceBottom;
                QWidget* spaceLeft;
                QWidget* spaceRight;
                QWidget* spaceCenter;
            };

            void ApplyVerticalLayout();
            void ApplyHorizontalLayout();
            void AlignLeft();
            void AlignRight();
            void AlignHCenter();
            void AlignTop();
            void AlignButtom();
            void AlignVCenter();

        protected:
            /// \brief 新建部件
            void onSetupUI() override;
            void onAttach(View *_child) override;
            void onDetach(View *_child) override;
        private:
        };

        class FrameComponent : public jigsaw::core::MVCComponent
		{
        public:
            DECLARE_STATIC_NEW(Frame)
        protected:
            DECLARE_CONSTRUCTOR(Frame, MVC)
        };

    }//jigsaw
}//namespace
