#include "PluginInstallInfoMannager.h"
#include "common.h"
#include "IniReader.h"


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

    IniReader reader;
    std::string configFilePath = GetExecutablePath() + "/plugin_info.ini"; // 假设配置文件路径固定
    if (reader.load(configFilePath)) {
        auto sections = reader.getSections();
        for (const auto& section : sections) {
            auto sectionData = reader.getSectionData(section);
            PluginInfo info;
            info.name = section;
            info.description = reader.getValue(section, "description", "");
            info.version = reader.getValue(section, "version", "");
            info.author = reader.getValue(section, "author", "");
            info.running = false;
            m_mapPluginInfo[info.name] = info;
        }
    }
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
    IniReader reader;
    if (reader.load(configFilePath)) {
        auto sections = reader.getSections();
        if (sections.empty())
        {
            return false;
        }
        
        info.name = sections[0];
        info.description = reader.getValue(sections[0], "description", "");
        info.version = reader.getValue(sections[0], "version", "");
        info.author = reader.getValue(sections[0], "author", "");
        info.running = false;
    }

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
