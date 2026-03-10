#include "PluginInstance.h"
#include "common.h"

// 万能查询通道的具体实现
void* QueryAPI(const char* apiName) {
    auto& registry = GetApiRegistry();
    auto it = registry.find(apiName);
    return (it != registry.end()) ? it->second : nullptr;
}

PluginInstance::PluginInstance()
{
}

PluginInstance::~PluginInstance()
{
    Unload();
}

bool PluginInstance::LoadByPath(const char *pluginPath)
{
    Unload();
    m_handle = LOAD_LIB(pluginPath);
    if (!m_handle) return false;

    // 1. 获取三个核心入口点
    auto setupFunc = (PFN_SetupPluginAPI)GET_FUNC(m_handle, "SetupPluginAPI");
    m_startFunc = (PFN_StartPlugin)GET_FUNC(m_handle, "StartPlugin");
    m_stopFunc = (PFN_StopPlugin)GET_FUNC(m_handle, "StopPlugin");

    if (!setupFunc || !m_startFunc || !m_stopFunc) {
        CLOSE_LIB(m_handle);
        return false;
    }

    // 2. 注入系统的查询通道
    setupFunc(QueryAPI);

    return true;
}

bool PluginInstance::Load(const char *pluginId)
{
    if(!PluginInfoManager::GetPluginInfo(pluginId, m_pluginInfo))
    {
        return false; // 解析失败
    }
    std::string pluginPath = GetExecutablePath() + "/\\" + m_pluginInfo.id + "/\\" + m_pluginInfo.name + ".dll";
    return LoadByPath(pluginPath.c_str());
}

void PluginInstance::Unload()
{
    Stop();
    if (m_handle) {
        CLOSE_LIB(m_handle);
        m_handle = nullptr;
    }
}

bool PluginInstance::Start()
{
    if (m_pluginInfo.name.empty())
    {
        return false;
    }
    
    if (m_pluginInfo.running)
    {
        return true; // 已经在运行了
    }
    if (!m_handle || !m_startFunc || !m_stopFunc) {
        CLOSE_LIB(m_handle);
        return false;
    }
    
    std::string pluginPath = GetExecutablePath() + "/\\" + m_pluginInfo.id + "/\\" + m_pluginInfo.name + ".dll";
    m_pluginInfo.running = true;
    PluginInfoManager::SetPluginInfo(m_pluginInfo);
    // 3. 启动插件业务逻辑
    m_startFunc();
    return m_pluginInfo.running;
}

void PluginInstance::Stop()
{
    if (m_handle && m_stopFunc) {
        m_stopFunc();
        m_pluginInfo.running = false;
        PluginInfoManager::SetPluginInfo(m_pluginInfo);
    }
}
