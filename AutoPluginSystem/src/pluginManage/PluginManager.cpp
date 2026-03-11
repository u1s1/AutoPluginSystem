#include "PluginManager.h"
#include <fstream>
#include <iostream>
#include "common.h"
#include "InstallOperator.h"

PluginManager::PluginManager(std::unique_ptr<IPluginInstaller> installer, 
    std::shared_ptr<PluginInfoManager> infoManager) : 
    m_installer(std::move(installer)),
    m_infoManager(infoManager)
{
    auto pluginList = m_infoManager->GetPluginList();
    for (const auto& pluginInfo : pluginList) {
        m_mapInstance[pluginInfo.id] = nullptr;
    }
}
PluginManager::~PluginManager()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& instance : m_mapInstance)
    {
        if (instance.second != nullptr)
        {
            instance.second->Unload();
            instance.second.reset();
        }
    }
    
}

bool PluginManager::Install(const std::string& pluginPath, bool allCopy)
{
    PluginInfo pluginInfo;
    if (m_installer->InstallPlugin(pluginPath, pluginInfo, allCopy) != 0)
    {
        return false;
    }
    Unload(pluginInfo.id.c_str());
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapInstance[pluginInfo.id] = nullptr; // 先注册一个空实例，等到 Load 时再创建真正的实例
    return true;
}

void PluginManager::Uninstall(const std::string& pluginID)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_mapInstance.find(pluginID);
        if (it == m_mapInstance.end())
        {
            return;
        }
        if (it->second != nullptr)
        {
           it->second->Unload();
        }
        m_mapInstance.erase(it);
    }
    m_installer->UninstallPlugin(pluginID);
}

bool PluginManager::LoadByPath(const std::string& pluginID, const std::string& pluginPath)
{
    Unload(pluginID);
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapInstance[pluginID] = std::make_shared<PluginInstance>(m_infoManager);
    m_mapInstance[pluginID]->LoadByPath(pluginPath);
    return true;
}

bool PluginManager::Load(const std::string& pluginID)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_mapInstance.find(pluginID) == m_mapInstance.end())
        {
            return false;
        }
    }
    Unload(pluginID);
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapInstance[pluginID] = std::make_shared<PluginInstance>(m_infoManager);
    m_mapInstance[pluginID]->Load(pluginID);
    return true;
}

bool PluginManager::Unload(const std::string& pluginID)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_mapInstance.find(pluginID) == m_mapInstance.end())
    {
        return true; //已经没有
    }
    if (m_mapInstance[pluginID] != nullptr)
    {
        m_mapInstance[pluginID]->Unload();
    }
    m_mapInstance[pluginID] = nullptr;
    return true;
}

bool PluginManager::Start(const std::string& pluginID)
{
    std::shared_ptr<PluginInstance> instanceStart = nullptr;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_mapInstance.find(pluginID) == m_mapInstance.end())
        {
            return false;
        }
        if (m_mapInstance[pluginID] == nullptr)
        {
            return false;
        }
        instanceStart = m_mapInstance[pluginID];
    }
    return instanceStart->Start();
}

void PluginManager::Stop(const std::string& pluginID)
{
    std::shared_ptr<PluginInstance> instanceStop = nullptr;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_mapInstance.find(pluginID) == m_mapInstance.end())
        {
            return;
        }
        if (m_mapInstance[pluginID] == nullptr)
        {
            return;
        }
        instanceStop = m_mapInstance[pluginID];
    }
    instanceStop->Stop();
}
