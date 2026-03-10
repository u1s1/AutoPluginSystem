#include "PluginManager.h"
#include <fstream>
#include <iostream>
#include "common.h"
#include "InstallOperator.h"

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
    Stop();
}

bool PluginManager::Install(const char *pluginPath, bool allCopy)
{
    PluginInfo pluginInfo;
    if(!PluginInfoMannager::GetPluginInfoFromPath(pluginPath, pluginInfo))
    {
        std::cout << "analysis failed" << std::endl;
        return false; // 解析失败
    }
    
    if (PluginInfoMannager::RegisterPluginInfo(pluginInfo) != 0)
    {
        std::cout << "register failed" << std::endl;
        return false; // 注册失败
    }

    return InstallOperator::InstallPlugin(pluginPath, pluginInfo, allCopy) == 0;
}

void PluginManager::Uninstall()
{
    if (m_handle && m_stopFunc && m_uninstallFunc) {
        m_stopFunc();
        m_uninstallFunc();
        CLOSE_LIB(m_handle);
        m_handle = nullptr;
    }
    //下面执行删除插件文件及收尾操作
    /* */
    InstallOperator::UninstallPlugin(m_pluginInfo.name.c_str());
    PluginInfoMannager::DeletePluginInfo(m_pluginInfo.name.c_str());
}

bool PluginManager::LoadAndStart(const char *pluginPath)
{
    m_handle = LOAD_LIB(pluginPath);
    if (!m_handle) return false;

    // 1. 获取三个核心入口点
    auto setupFunc = (PFN_SetupPluginAPI)GET_FUNC(m_handle, "SetupPluginAPI");
    auto startFunc = (PFN_StartPlugin)GET_FUNC(m_handle, "StartPlugin");
    m_stopFunc = (PFN_StopPlugin)GET_FUNC(m_handle, "StopPlugin");
    m_uninstallFunc = (PFN_UninstallPlugin)GET_FUNC(m_handle, "UninstallPlugin");

    if (!setupFunc || !startFunc || !m_stopFunc || !m_uninstallFunc) {
        CLOSE_LIB(m_handle);
        return false;
    }

    // 2. 注入系统的查询通道
    setupFunc(QueryAPI);

    // 3. 启动插件业务逻辑
    startFunc();

    return true;
}

bool PluginManager::Start(const char *pluginName)
{
    if(!PluginInfoMannager::GetPluginInfo(pluginName, m_pluginInfo))
    {
        return false; // 解析失败
    }
    if (m_pluginInfo.running)
    {
        return true; // 已经在运行了
    }
    
    std::string pluginPath = GetExecutablePath() + "/\\" + m_pluginInfo.name + "/\\" + m_pluginInfo.name + ".dll";
    m_pluginInfo.running = LoadAndStart(pluginPath.c_str());
    PluginInfoMannager::SetPluginInfo(m_pluginInfo);
    return m_pluginInfo.running;
}

void PluginManager::Stop()
{
    if (m_handle && m_stopFunc) {
        m_stopFunc();
        CLOSE_LIB(m_handle);
        m_handle = nullptr;
        m_pluginInfo.running = false;
        PluginInfoMannager::SetPluginInfo(m_pluginInfo);
    }
}
