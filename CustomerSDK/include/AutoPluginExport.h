#ifndef AUTOPLUGINEXPORT_H
#define AUTOPLUGINEXPORT_H

#if defined(_WIN32)
    #define PLUGIN_EXPORT __declspec(dllexport)
#else
    #define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

// 声明内部初始化函数（实现在 SDK 静态库里）
void Internal_InitHostAPI(void* queryFunc);

// 给开发者用的语法糖，强行生成 SetupPluginAPI 入口点
#define EXPORT_PLUGIN_ENTRY() \
    extern "C" PLUGIN_EXPORT void SetupPluginAPI(void* queryFunc) { \
        Internal_InitHostAPI(queryFunc); \
    }

#endif