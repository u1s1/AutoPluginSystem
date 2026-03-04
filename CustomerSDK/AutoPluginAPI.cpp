#include "AutoPluginAPI.h"
#include <stdio.h> // 引入底层 C 打印

// 隐藏的全局查询函数指针
static PFN_QueryHostAPI g_queryApi = nullptr;

// 供宏调用的内部初始化函数
void Internal_InitHostAPI(void* queryFunc) {
    g_queryApi = reinterpret_cast<PFN_QueryHostAPI>(queryFunc);
}

typedef void (*PFN_AuoPluginLog_Impl)(LogLevel, const char*);
void AutoPluginLog(LogLevel level, const char *message)
{
    if (!g_queryApi)
    {
        printf("[Plugin SDK Error] g_queryApi is NULL! SetupPluginAPI was not called.\n");
        return; // 防御
    } 
    
    // 性能核心：static 变量只在第一次调用时初始化查表！
    static auto real_func = reinterpret_cast<PFN_AuoPluginLog_Impl>(g_queryApi("AutoPluginLog"));
    
    // 之后的所有调用，直接通过函数指针跳转，零查表开销
    if (real_func) {
        real_func(level, message);
    } else {
        printf("[Plugin SDK Error] Cannot find 'SystemLog' in Host Registry!\n");
    }
}