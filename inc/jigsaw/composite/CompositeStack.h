#pragma once

#include "CompositePage.h"

namespace jigsaw
{
	namespace composite
	{
        class StackView : public PageView
		{
		public:
		protected:
            /// \brief 新建部件
            void onSetupUI() override;
        private:
		};


        class PageComponent;

        /// \note StackComponent在Release时将移除并销毁所有子组件
        class StackComponent : public PageComponent
		{
		public:
            DECLARE_STATIC_NEW(Stack)
            FORBID_ADDMVCCHILD

            PageComponent* NewPage();
            PageComponent* NewPage(const QString& _uuid);
			void BringForward(const QString& _uuid);

        protected:
            DECLARE_CONSTRUCTOR(Stack, Page)
            void onInitialize() override;
            void onRelease() override;
		};
	}
}
