#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
    namespace composite
	{
        class MaskView: public jigsaw::core::View
        {
        public:
            void Popup();
            void Close();
        protected:
            /// \brief 新建部件
            void onSetupUI() override;
            void onAttach(View *_child) override;
            void onDetach(View *_child) override;
        private:
        };

        class MaskComponent : public jigsaw::core::MVCComponent
		{
        public:
            DECLARE_STATIC_NEW(Mask)
        protected:
            DECLARE_CONSTRUCTOR(Mask, MVC)
        };

    }//jigsaw
}//namespace
