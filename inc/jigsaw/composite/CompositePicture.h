#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
	namespace composite
	{
		class PictureView : public jigsaw::core::View
		{
		public:
			struct PictureElement : public jigsaw::core::View::Element
			{
                QLabel* ptrPicture;
			};

			void SetPNG(const QString& _path);
            void SetJPG(const QString& _path);
            void SetGIF(const QString& _path);
            void SetFitJPG(const QString& _path);
            void LoadFromJPG(const QByteArray& _data);
            void LoadFromPNG(const QByteArray& _data);
            void LoadFromGIF(const QByteArray& _data);
		protected:
            /// \brief 新建部件
			void onSetupUI() override;
            void onReset() override;
		private:
		};

		class PictureComponent : public jigsaw::core::MVCComponent
		{
		public:
            DECLARE_STATIC_NEW(Picture)
            FORBID_ADDMVCCHILD
        protected:
            DECLARE_CONSTRUCTOR(Picture, MVC)
		};
	}
}
