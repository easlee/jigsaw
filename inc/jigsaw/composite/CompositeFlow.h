#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
    namespace composite
	{
        class FlowView: public jigsaw::core::View
        {
        public:
            void SetMargin(int _margin);
            void SetHSpacing(int _spacing);
            void SetVSpacing(int _spacing);
        protected:
            /// \brief 新建部件并设定私有Flow布局
            void onSetupUI() override;
        private:
        };

        class FlowComponent : public jigsaw::core::MVCComponent
		{
        public:
            DECLARE_STATIC_NEW(Flow)
        protected:
            DECLARE_CONSTRUCTOR(Flow, MVC)
        };

    }//jigsaw
}//namespace
