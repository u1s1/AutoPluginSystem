#include "PluginInstallInfoMannager.h"


PluginInstallInfoMannager::PluginInstallInfoMannager()
{
    Init();
}

PluginInstallInfoMannager::~PluginInstallInfoMannager()
{
}

void PluginInstallInfoMannager::Init()
{
    //解析配置文件，填充 m_mapPluginInfo
    m_mapPluginInfo.clear();
}


bool PluginInstallInfoMannager::GetPluginInfoFromPath(const char * pluginPath, PluginInfo & info)
{
    std::string strPluginPath(pluginPath);
    size_t lastSlash = strPluginPath.find_last_of("/\\");
    if (lastSlash == std::string::npos) {
        return false; // 无效路径
    }
    strPluginPath = strPluginPath.substr(0, lastSlash + 1); // 获取目录
    std::string configFilePath = strPluginPath + "plugin.config"; // 假设配置文件名固定
    // 解析 configFilePath，填充 info

    return true;
}

int PluginInstallInfoMannager::RegisterPluginInfo(const PluginInfo & info)
{
    if (info.name.empty())
    {
        return 3;
    }
    if (getInstance().m_mapPluginInfo.find(info.name) != getInstance().m_mapPluginInfo.end())
    {
        return 1; // 已存在同名插件
    }

    getInstance().m_mapPluginInfo[info.name] = info;
    //接下来保存新信息到配置文件

    return 0; // 注册成功
}

bool PluginInstallInfoMannager::GetPluginInfo(const char *pluginName, PluginInfo &info)
{
    if (info.name.empty())
    {
        return 3;
    }
    if (getInstance().m_mapPluginInfo.find(pluginName) == getInstance().m_mapPluginInfo.end())
    {
        return false;
    }
    info = getInstance().m_mapPluginInfo[pluginName];
    
    return true;
}

bool PluginInstallInfoMannager::SetPluginInfo(const PluginInfo & info)
{
    if (info.name.empty())
    {
        return 3;
    }
    if (getInstance().m_mapPluginInfo.find(info.name) == getInstance().m_mapPluginInfo.end())
    {
        return false;
    }
    getInstance().m_mapPluginInfo[info.name] = info;
    //接下来保存新信息到配置文件
    
    return true;
}

bool PluginInstallInfoMannager::DeletePluginInfo(const char * pluginName)
{
    if (pluginName == nullptr || std::string(pluginName).empty())
    {
        return 3;
    }
    if (getInstance().m_mapPluginInfo.find(pluginName) == getInstance().m_mapPluginInfo.end())
    {
        return true; // 已经不存在了
    }
    //接下来保存已经删除插件的新信息到配置文件

    
    getInstance().m_mapPluginInfo.erase(pluginName);
    return true;
}
