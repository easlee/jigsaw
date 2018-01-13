#pragma once

#include "CompositePane.h"

namespace jigsaw
{
    namespace composite
	{

        class FollowBarView: public PaneView
        {
        public:
            struct FollowBarElement : public jigsaw::core::View::Element
            {
                QWidget* ptrSlotTools;
                QWidget* ptrToolbar;
                QWidget* ptrSpacer;
            };
        protected:
            void onAttach(View* _child) override;
            void onDetach(View *_child) override;
        private:
        };

        class VFollowBarView : public FollowBarView
        {
        protected:
            /// \brief 加载vfollowbar.ui
            void onSetupUI() override;
        };

        /// \note 加上TAG_TOOL标签的组件将被放入工具栏
        class FollowBarComponent : public jigsaw::composite::PaneComponent
		{
        public:
            const static QString TAG_TOOL;
            static FollowBarComponent* NewV();
            static FollowBarComponent* NewV(const QString& _uuid);
        protected:
            DECLARE_CONSTRUCTOR(FollowBar, Pane)
        };

    }//jigsaw
}//namespace
