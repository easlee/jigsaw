#pragma once

#include "jigsaw/core/Composite.h"

namespace jigsaw
{
    namespace composite
    {
        class ComboboxView: public jigsaw::core::View
        {
        public:
            struct ComboboxElement : public jigsaw::core::View::Element
            {
                QComboBox *ptrComboxox;
            };

            std::function< void(const QString) > onSelected;
            void AddItem(QString _str);
            void AddItems(QStringList _lis);
        protected:
            /// \brief 加载combobox.ui
            void onSetupUI() override;
        private:

        };

        class ComboboxComponent : public jigsaw::core::MVCComponent
        {
        public:
            DECLARE_STATIC_NEW(Combobox)
            FORBID_ADDMVCCHILD
        protected:
            DECLARE_CONSTRUCTOR(Combobox, MVC)
        };

    }//jigsaw
}//namespace
