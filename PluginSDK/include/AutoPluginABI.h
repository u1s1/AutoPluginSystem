#ifndef AUTOPLUGINABI_H
#define AUTOPLUGINABI_H
// 核心契约：万能查询函数的函数指针类型
// 宿主实现它，插件调用它
typedef void* (*PFN_QueryHostAPI)(const char* apiName);

// 插件必须导出的生命周期函数类型
typedef void (*PFN_SetupPluginAPI)(PFN_QueryHostAPI queryFunc);
typedef bool (*PFN_StartPlugin)();
typedef void (*PFN_StopPlugin)();
typedef void (*PFN_UninstallPlugin)();

#endif