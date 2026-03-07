#include "AutoPluginAPI.h"
#include <stdio.h> // 引入底层 C 打印

// 隐藏的全局查询函数指针
static PFN_QueryHostAPI g_queryApi = nullptr;

// 供宏调用的内部初始化函数
void Internal_InitHostAPI(void* queryFunc) {
    g_queryApi = reinterpret_cast<PFN_QueryHostAPI>(queryFunc);
}

#define INVOKE_HOST_API_RET(ApiName, FuncType, ...) \
    if (!g_queryApi) { \
        printf("[Plugin SDK Error] g_queryApi is NULL!\n"); \
        return FuncType(); /* 返回一个默认构造的值，防止报错 */ \
    } \
    static auto real_func = reinterpret_cast<FuncType>(g_queryApi(#ApiName)); \
    if (real_func) { \
        return real_func(__VA_ARGS__); \
    } else { \
        printf("[Plugin SDK Error] Cannot find '%s'\n", #ApiName); \
        return FuncType(); \
    }

#define INVOKE_HOST_API_VOID(ApiName, FuncType, ...) \
    if (!g_queryApi) { \
        printf("[Plugin SDK Error] g_queryApi is NULL!\n"); \
    } \
    static auto real_func = reinterpret_cast<FuncType>(g_queryApi(#ApiName)); \
    if (real_func) { \
        real_func(__VA_ARGS__); \
    } else { \
        printf("[Plugin SDK Error] Cannot find '%s'\n", #ApiName); \
    }

typedef void (*PFN_AuoPluginLog_Impl)(const char*);
void AutoPluginLog(const char *message)
{
    INVOKE_HOST_API_VOID(AutoPluginLog, PFN_AuoPluginLog_Impl, message);
}