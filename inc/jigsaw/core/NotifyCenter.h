#pragma once

#include <functional>
#include <QString>

class QWidget;

namespace jigsaw
{
namespace core
{

///
/// \brief The NotifyCenter class
///
/// \code
/// NotifyCenter::onInfo = [](const QString& _text) {
///     qDebug() << _text;
/// };
/// NotifyCenter::onFatal = [](const QString& _text) {
///     NotifyCenter::Popup(NotifyCenter::Style::DANGER, _text);
/// };
///
class NotifyCenter
{
public:
    enum Style
    {
        PRIMARY,
        INFO,
        SUCCESS,
        WARNING,
        DANGER
    };


    static std::function<void(const QString&)> onInfo;
    static std::function<void(const QString&)> onAlert;
    static std::function<void(const QString&)> onFatal;

    static void Initialize();

    /// \brief 断开所有回调
    static void Reset();
    /// \brief 重新调整大小，铺满父窗口
    static void Resize();

    static void Info(const QString& _text);
    static void Alert(const QString& _text);
    static void Fatal(const QString& _text);
    static void Assert(bool _expression, const QString& _text);

    static void Popup(Style _style, const QString _text, QWidget* _parent = 0);

};

}//namesapce core
}//namespace jigsaw
