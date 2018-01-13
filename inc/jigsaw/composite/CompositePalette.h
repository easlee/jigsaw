#pragma once

#include "jigsaw/core/ColorPalette.h"
#include "jigsaw/core/Composite.h"

namespace jigsaw
{
    namespace composite
	{
        /// 使用Flow布局
        class PaletteView : public jigsaw::core::View
        {
        public:
            std::function<void (const QString& _hex) > onPick;
        protected:
            /// \brief 加载palette.ui并绑定事件
            void onSetupUI() override;
        private:
            void handleRepaint(const jigsaw::core::Data* _data);
        };

        class PaletteComponentInternal;

        class PaletteComponent : public jigsaw::core::MVCComponent
		{
            friend class PaletteComponentInternal;
        public:
            DECLARE_STATIC_NEW(Palette)
            FORBID_ADDMVCCHILD
        protected:
            DECLARE_CONSTRUCTOR(Palette, MVC)
            void onInitialize() override;
            void onStartup() override;
            void onRelease() override;
        private:
            PaletteComponentInternal* internal_;
        };

    }//jigsaw
}//namespace
