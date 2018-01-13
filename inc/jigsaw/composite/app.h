#pragma once
#include <functional>
#include <QSize>

namespace jigsaw
{

namespace core
{
    class MVCComponent;
}//namespace core

namespace composite
{

class AppInternal;

class App
{
public:
    /// \brief load font,translator,palette,notify
    static void Boot();

    /// \note Run会调用_root的Reset(true)和Refresh(true),将左右组件先重置为无数据的状态，再使用绑定的数据进行一次刷新
    static void Run(core::MVCComponent* _root, const QSize& _size, bool _isMaximized = false);

    static void AppendHeaderTool(core::MVCComponent* _component);
};


}//composite
}//namespace jigsaw
