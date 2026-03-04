#include "../include/AutoPluginManager.h"

// 万能查询通道的具体实现
void* QueryAPI(const char* apiName) {
    auto& registry = GetApiRegistry();
    auto it = registry.find(apiName);
    return (it != registry.end()) ? it->second : nullptr;
}

PluginManager::PluginManager()
{
}
PluginManager::~PluginManager()
{
    Unload();
}

bool PluginManager::LoadAndStart(const char *pluginPath)
{
    m_handle = LOAD_LIB(pluginPath);
    if (!m_handle) return false;

    // 1. 获取三个核心入口点
    auto setupFunc = (PFN_SetupPluginAPI)GET_FUNC(m_handle, "SetupPluginAPI");
    auto startFunc = (PFN_StartPlugin)GET_FUNC(m_handle, "StartPlugin");
    m_stopFunc = (PFN_StopPlugin)GET_FUNC(m_handle, "StopPlugin");

    if (!setupFunc || !startFunc || !m_stopFunc) {
        CLOSE_LIB(m_handle);
        return false;
    }

    // 2. 注入系统的查询通道
    setupFunc(QueryAPI);

    // 3. 启动插件业务逻辑
    return startFunc();
}

void PluginManager::Unload()
{
    if (m_handle && m_stopFunc) {
        m_stopFunc();
        CLOSE_LIB(m_handle);
        m_handle = nullptr;
    }
}