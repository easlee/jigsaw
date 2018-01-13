#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
	namespace composite
	{
        class ImageButtonView : public jigsaw::core::View
		{
		public:
            struct ImageButtonElement :public jigsaw::core::View::Element
			{
                QPushButton* ptrButton;
			};
			
            std::function< void(ImageButtonView*) > onClick;
            void SetNormalImage(const QString& _image);
            void SetHoverImage(const QString& _image);
            void SetPressedImage(const QString& _image);
            void SetDisabledImage(const QString& _image);
            void SetNormalImage(const QString& _image, const QRect& _imageBorder, const QRect& _buttonBorder);
            void SetHoverImage(const QString& _image, const QRect& _imageBorder, const QRect& _buttonBorder);
            void SetPressedImage(const QString& _image, const QRect& _imageBorder, const QRect& _buttonBorder);
            void SetDisabledImage(const QString& _image, const QRect& _imageBorder, const QRect& _buttonBorder);
        protected:
            /// \brief 新建部件并绑定事件
			void onSetupUI() override;
			void handleOnApplyTheme(jigsaw::core::IAction* _action) override;
		private:
            void updateQSS();
            QString qssNormal;
            QString qssHover;
            QString qssPressed;
            QString qssDisabled;
		};

        class ImageButtonComponent : public jigsaw::core::MVCComponent
		{
		public:
            DECLARE_STATIC_NEW(ImageButton)
            FORBID_ADDMVCCHILD
        protected:
            DECLARE_CONSTRUCTOR(ImageButton, MVC)
		};
	}
}
