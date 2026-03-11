#include "PluginInfoManager.h"
#include "common.h"
#include "IniOperator.h"


PluginInfoManager::PluginInfoManager()
{
    Init();
}

PluginInfoManager::~PluginInfoManager()
{
    Save();
}

void PluginInfoManager::Init()
{
    //解析配置文件，填充 m_mapPluginInfo
    m_mapPluginInfo.clear();

    IniOperator reader;
    std::string configFilePath = GetExecutablePath() + "/plugin_info.ini"; // 假设配置文件路径固定
    if (reader.load(configFilePath)) {
        auto sections = reader.getAllSections();
        for (const auto& section : sections) {
            PluginInfo info;
            info.id = section;
            info.name = reader.getValue(section, "name", "");
            info.description = reader.getValue(section, "description", "");
            info.version = reader.getValue(section, "version", "");
            info.author = reader.getValue(section, "author", "");
            info.running = false;
            m_mapPluginInfo[info.id] = info;
        }
    }
    
}

void PluginInfoManager::Save()
{
    IniOperator writer(GetExecutablePath() + "/plugin_info.ini");
    for (const auto& pair : getInstance().m_mapPluginInfo) {
        writer.setSectionData(pair.second);
    }
    writer.save();
}

bool PluginInfoManager::GetPluginInfoFromPath(const std::string&  pluginPath, PluginInfo & info)
{
    std::string strPluginPath(pluginPath);
    size_t lastSlash = strPluginPath.find_last_of("/\\");
    if (lastSlash == std::string::npos) {
        return false; // 无效路径
    }
    strPluginPath = strPluginPath.substr(0, lastSlash + 1); // 获取目录
    std::string configFilePath = strPluginPath + "plugin_info.ini"; // 假设配置文件名固定
    // 解析 configFilePath，填充 info
    IniOperator reader;
    if (!reader.load(configFilePath)) {
        return false;
    }
    auto sections = reader.getAllSections();
    if (sections.empty())
    {
        return false;
    }
    
    info.id = sections[0];
    info.name = reader.getValue(sections[0], "name", "");
    info.description = reader.getValue(sections[0], "description", "");
    info.version = reader.getValue(sections[0], "version", "");
    info.author = reader.getValue(sections[0], "author", "");
    info.running = false;

    return true;
}

int PluginInfoManager::RegisterPluginInfo(const PluginInfo & info)
{
    if (info.id.empty())
    {
        return 3; //待安装插件信息为空
    }
    if (getInstance().m_mapPluginInfo.find(info.id) != getInstance().m_mapPluginInfo.end())
    {
        if (getInstance().m_mapPluginInfo[info.id].author != info.author)
        {
            return 4; // 已存在同名插件但作者不同
        }
        
        if (getInstance().m_mapPluginInfo[info.id].version >= info.version)
        {
            return 2; // 已存在同名插件且版本不低于当前版本
        }
    }

    getInstance().m_mapPluginInfo[info.id] = info;
    //接下来保存新信息到配置文件
    getInstance().Save();

    return 0; // 注册成功
}

bool PluginInfoManager::GetPluginInfo(const std::string& pluginID, PluginInfo &info)
{
    if (pluginID.empty())
    {
        return false;
    }
    if (getInstance().m_mapPluginInfo.find(pluginID) == getInstance().m_mapPluginInfo.end())
    {
        return false;
    }
    info = getInstance().m_mapPluginInfo[pluginID];
    
    return true;
}

bool PluginInfoManager::SetPluginInfo(const PluginInfo & info)
{
    if (info.id.empty())
    {
        return 3;
    }
    if (getInstance().m_mapPluginInfo.find(info.id) == getInstance().m_mapPluginInfo.end())
    {
        return false;
    }
    getInstance().m_mapPluginInfo[info.id] = info;
    //接下来保存新信息到配置文件
    getInstance().Save();
    
    return true;
}

bool PluginInfoManager::DeletePluginInfo(const std::string&  pluginID)
{
    if (pluginID.empty())
    {
        return 3;
    }
    if (getInstance().m_mapPluginInfo.find(pluginID) == getInstance().m_mapPluginInfo.end())
    {
        return true; // 已经不存在了
    }
    getInstance().m_mapPluginInfo.erase(pluginID);
    //接下来保存已经删除插件的新信息到配置文件
    getInstance().Save();
    
    return true;
}

std::vector<PluginInfo> PluginInfoManager::GetPluginList()
{
    std::vector<PluginInfo> pluginList;
    for (const auto& pair : getInstance().m_mapPluginInfo) {
        pluginList.push_back(pair.second);
    }
    return pluginList;
}
