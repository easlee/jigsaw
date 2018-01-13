#include <assert.h>
#include <stdlib.h>
#include <QDebug>
#include <QApplication>
#include <QUiLoader>
#include <QDialog>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include "jigsaw/core/ColorPalette.h"
#include "jigsaw/core/NotifyCenter.h"

std::function<void(const QString&)> jigsaw::core::NotifyCenter::onInfo;
std::function<void(const QString&)> jigsaw::core::NotifyCenter::onAlert;
std::function<void(const QString&)> jigsaw::core::NotifyCenter::onFatal;

namespace jigsaw
{
namespace core
{

class NotifyCenterInternal
{
public:
    static QDialog* dialog;
    static QLabel* text;
    static QString qss;
};

QDialog* NotifyCenterInternal::dialog = 0;
QLabel* NotifyCenterInternal::text = 0;
QString NotifyCenterInternal::qss = "";

} //namespace core
} //namespace jigsaw

void jigsaw::core::NotifyCenter::Initialize()
{
    QUiLoader loader;
    QFile file(":/jigsaw/ui/notify");
    if(!file.open(QFile::ReadOnly))
        return;

    QWidget* root = dynamic_cast<QDialog*>(loader.load(&file));
    root->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    root->setAttribute(Qt::WA_TranslucentBackground);
    QPushButton* close = root->findChild<QPushButton*>("__btnClose");
    QLabel* text = root->findChild<QLabel*>("__text");
    QObject::connect(close, &QPushButton::clicked, [](){
       NotifyCenterInternal::dialog->accept();
    });

    NotifyCenterInternal::dialog = dynamic_cast<QDialog*>(root);
    NotifyCenterInternal::text = text;
    NotifyCenterInternal::qss = root->styleSheet();
}

void jigsaw::core::NotifyCenter::Reset()
{
   onInfo = 0;
   onAlert = 0;
   onFatal = 0;
}

void jigsaw::core::NotifyCenter::Resize()
{
    auto dialog = NotifyCenterInternal::dialog;
    if(0 == dialog->parentWidget())
        return;

    dialog->setGeometry(QRect(0, 0, dialog->parentWidget()->width(), dialog->parentWidget()->height()));
}

void jigsaw::core::NotifyCenter::Info(const QString &_text)
{
    if(0 == onInfo)
        return;
    onInfo(_text);
}

void jigsaw::core::NotifyCenter::Alert(const QString &_text)
{
    if(0 == onAlert)
        return;
    onAlert(_text);
}

void jigsaw::core::NotifyCenter::Fatal(const QString &_text)
{
    qDebug() << "FATAL: " << _text;
    if(0 != onFatal)
        onFatal(_text);
    //TODO 根据QT是否进入了事件循环选择退出方式
    std::exit(0);
}

void jigsaw::core::NotifyCenter::Assert(bool _expression, const QString &_text)
{
    assert(_expression);
    if(_expression)
        return;
    Fatal(_text);
}

void jigsaw::core::NotifyCenter::Popup(jigsaw::core::NotifyCenter::Style _style, const QString _text, QWidget* _parent /*=0*/)
{
    if(0 == NotifyCenterInternal::dialog)
        return;

    QString hex = "#ffffff";
    if(Style::PRIMARY == _style)
        hex = jigsaw::core::ColorPalette::Primary().Normal().Hex();
    else if(Style::INFO == _style)
        hex = jigsaw::core::ColorPalette::Info().Normal().Hex();
    else if(Style::SUCCESS == _style)
        hex = jigsaw::core::ColorPalette::Success().Normal().Hex();
    else if(Style::WARNING == _style)
        hex = jigsaw::core::ColorPalette::Warning().Normal().Hex();
    else if(Style::DANGER == _style)
        hex = jigsaw::core::ColorPalette::Danger().Normal().Hex();

    QString qss = NotifyCenterInternal::qss;
    qss.append(QString("QWidget#__banner { background: %1; }").arg(hex));

    NotifyCenterInternal::dialog->setStyleSheet(qss);
    if(0 != _parent)
    {
        QPoint pos = _parent->mapToGlobal(QPoint(0, 0));
        //NotifyCenterInternal::dialog->setParent(_parent);
        NotifyCenterInternal::dialog->setGeometry(QRect(pos.x(), pos.y(), _parent->width(), _parent->height()));
    }
    else
    {
        NotifyCenterInternal::dialog->setFixedSize(QSize(320, 240));
    }
    NotifyCenterInternal::text->setText(_text);
    NotifyCenterInternal::dialog->exec();
}

