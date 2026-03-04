// 📁 MyPlugin/main.cpp
#include <AutoPluginAPI.h>

// 1. 一行代码搞定底层注入
EXPORT_PLUGIN_ENTRY()

// 2. 插件启动逻辑
extern "C" PLUGIN_EXPORT bool StartPlugin() {
    
    // 直接像调原生 C 函数一样使用宿主功能！
    AutoPluginLog(LogLevel::Info, "Hello from Plugin! Static cache is amazing!");
    
    return true;
}

// 3. 插件停止逻辑
extern "C" PLUGIN_EXPORT void StopPlugin() {
    AutoPluginLog(LogLevel::Info, "Plugin is shutting down.");
}