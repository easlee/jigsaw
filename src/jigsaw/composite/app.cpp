#include <assert.h>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QApplication>
#include "jigsaw/core/ColorPalette.h"
#include "jigsaw/core/Translator.h"
#include "jigsaw/core/Composite.h"
#include "jigsaw/core/NotifyCenter.h"
#include "jigsaw/composite/App.h"

namespace jigsaw
{
namespace composite
{

class TitlebarWidget : public QWidget
{
public:
    TitlebarWidget(QWidget* _parent)
        :QWidget(_parent)
    {
        isPressed = false;
    }

    TitlebarWidget()
        :QWidget(NULL)
    {

    }

    void SetWindow(QMainWindow* _window)
    {
        window_ = _window;
    }

protected:
    void mousePressEvent(QMouseEvent* _event)
    {
        if (_event->button() == Qt::LeftButton)
        {
            assert(window_);
            isPressed = true;
            dragPosition = _event->globalPos() - window_->frameGeometry().topLeft();
            _event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent* _event)
    {
        if (!isPressed)
            return;

        if (_event->buttons() == Qt::LeftButton)
        {
            assert(window_);
            if(!window()->isMaximized())
                window_->move(_event->globalPos() - dragPosition);
            _event->accept();
        }
    }

    void mouseReleaseEvent(QMouseEvent*)
    {
        isPressed = false;
    }

private:
    QPoint dragPosition;
    QMainWindow* window_;
    bool isPressed;
};

class MainWindow : public QMainWindow
{
public:
    std::function<void()> OnResize;
protected:
    void resizeEvent(QResizeEvent* _event)
    {
        QMainWindow::resizeEvent(_event);
        if(0 != OnResize)
            OnResize();
    }
};

class AppInternal
{
public:
    static MainWindow* mainWindow;
    static QWidget* ptrRootContainer;
    static QCheckBox* ptrBtnMax;
    static QWidget* ptrToolbar;
    static jigsaw::core::MVCComponent* root;

    static void Boot()
    {
        //create mainWindow
        mainWindow = new MainWindow();
        mainWindow->setWindowFlags(Qt::FramelessWindowHint);
        ptrRootContainer = new QWidget(mainWindow);
        QVBoxLayout* ptrRootLayout = new QVBoxLayout(ptrRootContainer);
        ptrRootLayout->setMargin(0);
        ptrRootLayout->setSpacing(0);
        ptrRootContainer->setLayout(ptrRootLayout);
        mainWindow->setCentralWidget(ptrRootContainer);

        //create titlebar Shell
        TitlebarWidget* ptrTitleBarShell = new TitlebarWidget(mainWindow);
        ptrTitleBarShell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        ptrTitleBarShell->SetWindow(mainWindow);
        QHBoxLayout* ptrTitlebarLayout = new QHBoxLayout(ptrTitleBarShell);
        ptrTitlebarLayout->setSpacing(0);
        ptrTitlebarLayout->setMargin(0);
        ptrTitleBarShell->setLayout(ptrTitlebarLayout);
        ptrRootContainer->layout()->addWidget(ptrTitleBarShell);

        // create titlebar
        QWidget* root = core::View::NewRoot(":/jigsaw/ui/titlebar");
        assert(root);
        ptrToolbar = root->findChild<QWidget*>("__slotItems");

        QPushButton* btnMin = root->findChild<QPushButton*>("__btnMin");
        assert(btnMin);
        QObject::connect(btnMin, &QPushButton::clicked, [=] {
            mainWindow->showMinimized();
        });

        QPushButton* btnClose = root->findChild<QPushButton*>("__btnClose");
        assert(btnClose);
        QObject::connect(btnClose, &QPushButton::clicked, [=] {
            QApplication::quit();
        });

        ptrBtnMax = root->findChild<QCheckBox*>("__cbMax");
        assert(ptrBtnMax);
        QObject::connect(ptrBtnMax, &QCheckBox::clicked, [=] {
            if (mainWindow->isMaximized())
                mainWindow->showNormal();
            else
                mainWindow->showMaximized();
        });
        ptrTitleBarShell->layout()->addWidget(root);

        mainWindow->OnResize = [](){
            jigsaw::core::NotifyCenter::Resize();
        };
    }

    static void Run(core::MVCComponent* _root)
    {
        _root->GetView()->element()->root->setParent(AppInternal::ptrRootContainer);
        ptrRootContainer->layout()->addWidget(_root->GetView()->element()->root);
        jigsaw::core::NotifyCenter::Assert(_root->GetView()->element(), "the element of root component is null");
        jigsaw::core::NotifyCenter::Assert(_root->GetView()->element()->root, "the root of root component is null");
        _root->GetView()->element()->root->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
};

MainWindow* AppInternal::mainWindow = 0;
QCheckBox* AppInternal::ptrBtnMax = 0;
QWidget* AppInternal::ptrRootContainer = 0;
QWidget* AppInternal::ptrToolbar= 0;

}//namespace composite
}//namespace jigsaw

void jigsaw::composite::App::Boot()
{
    int id = QFontDatabase::addApplicationFont(":/jigsaw/font/default");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QApplication::setFont(QFont(family, 9));

    jigsaw::core::Translator::LoadSystemLanguage();

    jigsaw::core::ColorPalette::LoadColorPalette();

    jigsaw::core::NotifyCenter::Initialize();
    AppInternal::Boot();
}

void jigsaw::composite::App::Run(core::MVCComponent* _root, const QSize& _size, bool _isMaximized /*= false*/)
{
    AppInternal::Run(_root);
    AppInternal::mainWindow->show();
    AppInternal::mainWindow->resize(_size);
    if(_isMaximized)
    {
        AppInternal::ptrBtnMax->setChecked(true);
        AppInternal::mainWindow->showMaximized();
    }
    //处理一次事件循环，使窗体完成缩放
    QApplication::processEvents();
    //重置所有组件到无数据状态
    _root->Reset(true);
    //使用数据刷新所有组件
    _root->Refresh(true);
}

void jigsaw::composite::App::AppendHeaderTool(jigsaw::core::MVCComponent* _component)
{
    AppInternal::ptrToolbar->layout()->addWidget(_component->GetView()->element()->root);
}
