#include "jigsaw/core/NotifyCenter.h"
#include "jigsaw/composite/CompositePage.h"
#include "jigsaw/composite/CompositeStack.h"

void jigsaw::composite::StackView::onSetupUI()
{
   PageView::onSetupUI();
   QLayout* layout = new QHBoxLayout(element()->root);
   layout->setSpacing(0);
   layout->setMargin(0);
   element()->root->setLayout(layout);
}


jigsaw::composite::PageComponent* jigsaw::composite::StackComponent::NewPage()
{
    QString uuid = jigsaw::core::UUIDUtil::New();
    return NewPage(uuid);
}


//-----------------------------------------------------------------------
jigsaw::composite::PageComponent* jigsaw::composite::StackComponent::NewPage(const QString& _uuid)
{
    PageComponent* pageComponent = PageComponent::New(_uuid);
    pageComponent->GetView()->Hide();
    this->addMVCChild(pageComponent);
    return pageComponent;
}

//-----------------------------------------------------------------------
void jigsaw::composite::StackComponent::BringForward(const QString& _uuid)
{
    foreach (QString uuid, ListChildren())
    {
        auto pageComponent = dynamic_cast<PageComponent*>(this->FindChild(uuid));
        if(0 == pageComponent)
            continue;
        if(0 == pageComponent->GetUUID().compare(_uuid))
            pageComponent->GetView()->Show();
        else
            pageComponent->GetView()->Hide();
    }
}

void jigsaw::composite::StackComponent::onInitialize()
{

}

void jigsaw::composite::StackComponent::onRelease()
{
    foreach(QString child, ListChildren())
    {
        auto component = this->FindChild(child);
        this->RemoveMVCChild(child);
        delete component;
    }
}
