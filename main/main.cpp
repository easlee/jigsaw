#include <QApplication>
#include "jigsaw/core/Composite.h"
#include "jigsaw/composite/App.h"
#include "jigsaw/core/ColorPalette.h"
#include "jigsaw/core/Translator.h"
#include "jigsaw/composite/CompositeDrawer.h"
#include "jigsaw/composite/CompositeStack.h"
#include "jigsaw/composite/CompositePicture.h"
#include "jigsaw/composite/CompositeTextButton.h"

using namespace jigsaw::core;
using namespace jigsaw::composite;

class LoginView : public View
{
protected:
    void onSetupUI() override
    {
        element_ = new View::Element();
        element_->root = View::NewVRoot();
    }
};

class LoginComponent : public MVCComponent
{
public:
    DECLARE_STATIC_NEW(Login)
protected:
    DECLARE_CONSTRUCTOR(Login, MVC)
    void onInitialize() override
    {
    }
};

class RootView : public View
{
protected:
    void onSetupUI() override
    {
        element_ = new View::Element();
        element_->root = View::NewHRoot();
    }
};

class RootComponent : public MVCComponent
{
public:
    DECLARE_STATIC_NEW(Root)
protected:
    DECLARE_CONSTRUCTOR(Root, MVC)
    void onInitialize() override
    {
        auto ptrDrawerComponent = DrawerComponent::New();
        this->AddMVCChild(ptrDrawerComponent);

        auto ptrStackComponent = StackComponent::New();
        this->AddMVCChild(ptrStackComponent);

        ptrDrawerComponent->ToView<DrawerView>()->onTabClick =[=](const QString& _tabUUID) {
            ptrStackComponent->BringForward(_tabUUID);
        };

        // User Profile
        auto drawerHeaderComponent = PaneComponent::New();
        ptrDrawerComponent->AttachHeader(drawerHeaderComponent);
        auto drawerHeaderView =  drawerHeaderComponent->ToView<PaneView>();
        drawerHeaderView->ApplyHorizontalLayout();
        drawerHeaderView->element()->root->layout()->setMargin(10);
        auto userHeadComponent = PictureComponent::New();
        auto userHeadView = userHeadComponent->ToView<PictureView>();
        userHeadView->element()->root->setFixedSize(QSize(44, 44));
        userHeadView->SetPNG(":/app/picture/userhead");
        drawerHeaderComponent->AddMVCChild(userHeadComponent);
        drawerHeaderComponent->AddMVCChild(PaneComponent::New());
        auto signinComponent = TextButtonComponent::New();
        auto signinView = signinComponent->ToView<TextButtonView>();
        signinView->ApplySize(TextButtonView::SIZE_SMALL);
        signinView->ApplyStyle(ColorPalette::PRIMARY);
        signinView->SetText(TR("profile.signin"));
        signinView->element()->root->setFixedWidth(64);
        drawerHeaderComponent->AddMVCChild(signinComponent);

        auto addTabPage = [=](const QString& _tabText, const QString& _tabIcon, MVCComponent* _page)->QString {
            QString tabID = ptrDrawerComponent->NewTab(_tabText);
            ptrDrawerComponent->SetTabIcon(tabID, _tabIcon, QSize(44, 44));

            auto pageComponent = ptrStackComponent->NewPage(tabID);
            auto pageView = pageComponent->ToView<PageView>();
            pageComponent->AddMVCChild(_page);
            pageView->ApplyVerticalLayout();
            return tabID;
        };

        ptrDrawerComponent->NewSection(TR("drawer.section.examples"));
        // Create Application

        auto loginComponent  = LoginComponent::New();
        addTabPage(TR("drawer.examples.login"), ":/app/picture/drawer_comic", loginComponent);

        //Dispatcher::Handle<IndexComponentInternal, IndexActions::ExportXApp>(internal_, &IndexComponentInternal::HandleExportApp);
    }
    void onAwake() override
    {

    }
    void onRelease() override
    {

    }
};


int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("plugins");

    QApplication a(argc, argv);

    jigsaw::composite::App::Boot();

    auto root = RootComponent::New();
    root->Initialize(0);
    root->Awake();
    root->Startup();
    jigsaw::composite::App::Run(root, QSize(1024, 768), true);

    int res = a.exec();
    return res;
}
