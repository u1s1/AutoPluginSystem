// 📁 MyPlugin/main.cpp
#include <AutoPluginAPI.h>

// 一行代码搞定底层注入
EXPORT_PLUGIN_ENTRY()

// 插件启动逻辑
extern "C" PLUGIN_EXPORT bool StartPlugin() {
    
    // 直接像调原生 C 函数一样使用宿主功能！
    AutoPluginLog("Hello from Plugin! Static cache is amazing!FROM NEW TEMPLATE!");
    
    return true;
}

// 插件停止逻辑
extern "C" PLUGIN_EXPORT void StopPlugin() {
    AutoPluginLog("Plugin is shutting down.");
}

// 插件卸载逻辑
extern "C" PLUGIN_EXPORT void UninstallPlugin() {
    AutoPluginLog("Plugin is uninstall");
}