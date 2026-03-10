#include "PluginManager.h"
#include <fstream>
#include <iostream>
#include "common.h"
#include "InstallOperator.h"


PluginManager::PluginManager()
{
    auto pluginList = PluginInfoManager::GetPluginList();
    for (const auto& pluginInfo : pluginList) {
        m_mapInstance[pluginInfo.id] = nullptr;
    }
}
PluginManager::~PluginManager()
{
    for (auto& instance : m_mapInstance)
    {
        if (instance.second != nullptr)
        {
            instance.second->Unload();
            instance.second.reset();
        }
    }
    
}

bool PluginManager::Install(const char *pluginPath, bool allCopy)
{
    PluginInfo pluginInfo;
    if (InstallOperator::InstallPlugin(pluginPath, pluginInfo, allCopy) != 0)
    {
        return false;
    }
    getInstance().Unload(pluginInfo.id.c_str());
    getInstance().m_mapInstance[pluginInfo.id] = nullptr; // 先注册一个空实例，等到 Load 时再创建真正的实例
}

void PluginManager::Uninstall(const char *pluginID)
{
    if (getInstance().m_mapInstance.find(pluginID) == getInstance().m_mapInstance.end())
    {
        return;
    }
    getInstance().m_mapInstance[pluginID]->Unload();
    getInstance().m_mapInstance[pluginID] = nullptr;
    getInstance().m_mapInstance.erase(pluginID);
    InstallOperator::UninstallPlugin(pluginID);
}

bool PluginManager::LoadByPath(const char *pluginID, const char *pluginPath)
{
    getInstance().Unload(pluginID);
    getInstance().m_mapInstance[pluginID] = std::make_unique<PluginInstance>();
    getInstance().m_mapInstance[pluginID]->LoadByPath(pluginPath);
    return true;
}

bool PluginManager::Load(const char *pluginID)
{
    if (getInstance().m_mapInstance.find(pluginID) == getInstance().m_mapInstance.end())
    {
        return false;
    }
    getInstance().Unload(pluginID);
    getInstance().m_mapInstance[pluginID] = std::make_unique<PluginInstance>();
    getInstance().m_mapInstance[pluginID]->Load(pluginID);
    return true;
}

bool PluginManager::Unload(const char *pluginID)
{
    if (getInstance().m_mapInstance.find(pluginID) == getInstance().m_mapInstance.end())
    {
        return true; //已经没有
    }
    if (getInstance().m_mapInstance[pluginID] != nullptr)
    {
        getInstance().m_mapInstance[pluginID]->Unload();
    }
    getInstance().m_mapInstance[pluginID] = nullptr;
    return true;
}

bool PluginManager::Start(const char *pluginID)
{
    if (getInstance().m_mapInstance.find(pluginID) == getInstance().m_mapInstance.end())
    {
        return false;
    }
    if (getInstance().m_mapInstance[pluginID] == nullptr)
    {
        return false;
    }
    return getInstance().m_mapInstance[pluginID]->Start();
}

void PluginManager::Stop(const char *pluginID)
{
    if (getInstance().m_mapInstance.find(pluginID) == getInstance().m_mapInstance.end())
    {
        return;
    }
    if (getInstance().m_mapInstance[pluginID] == nullptr)
    {
        return;
    }
    getInstance().m_mapInstance[pluginID]->Stop();
}
