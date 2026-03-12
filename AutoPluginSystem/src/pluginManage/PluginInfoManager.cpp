#include "PluginInfoManager.h"
#include "common.h"
#include "IniOperator.h"


PluginInfoManager::PluginInfoManager(std::unique_ptr<IConfigParser> configer) :
    m_configParser(std::move(configer))
{
    Init();
}

PluginInfoManager::~PluginInfoManager()
{
    Save();
}

void PluginInfoManager::Init()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    //解析配置文件，填充 m_mapPluginInfo
    m_mapPluginInfo.clear();

    std::string configFilePath = GetExecutablePath() + "/plugin_info.ini"; // 假设配置文件路径固定
    if (!m_configParser->load(configFilePath)) {
        return;
    }
    auto sections = m_configParser->getAllSections();
    for (const auto& section : sections) {
        PluginInfo info;
        info.id = section;
        info.name = m_configParser->getValue(section, "name", "");
        info.description = m_configParser->getValue(section, "description", "");
        info.version = m_configParser->getValue(section, "version", "");
        info.author = m_configParser->getValue(section, "author", "");
        info.running = false;
        m_mapPluginInfo[info.id] = info;
    }
}

void PluginInfoManager::Save()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_configParser->load(GetExecutablePath() + "/plugin_info.ini"))
    {
        return;
    }
    for (const auto& pair : m_mapPluginInfo) {
        const PluginInfo& info = pair.second;
        // 业务层自己负责将 PluginInfo 转换为通用的 Map
        std::map<std::string, std::string> data;
        data["name"] = info.name;
        data["version"] = info.version;
        data["author"] = info.author;
        data["description"] = info.description;
        // 调用底层通用接口
        m_configParser->setSectionData(info.id, data);
    }
    m_configParser->save();
}

bool PluginInfoManager::GetPluginInfoFromPath(const std::string&  pluginPath, PluginInfo & info)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string strPluginPath(pluginPath);
    size_t lastSlash = strPluginPath.find_last_of("/\\");
    if (lastSlash == std::string::npos) {
        return false; // 无效路径
    }
    strPluginPath = strPluginPath.substr(0, lastSlash + 1); // 获取目录
    std::string configFilePath = strPluginPath + "plugin_info.ini"; // 假设配置文件名固定
    // 解析 configFilePath，填充 info
    if (!m_configParser->load(configFilePath)) {
        return false;
    }
    auto sections = m_configParser->getAllSections();
    if (sections.empty())
    {
        return false;
    }
    
    info.id = sections[0];
    info.name = m_configParser->getValue(sections[0], "name", "");
    info.description = m_configParser->getValue(sections[0], "description", "");
    info.version = m_configParser->getValue(sections[0], "version", "");
    info.author = m_configParser->getValue(sections[0], "author", "");
    info.running = false;

    return true;
}

int PluginInfoManager::RegisterPluginInfo(const PluginInfo & info, bool saveNow)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (info.id.empty())
        {
            return 3; //待安装插件信息为空
        }
        if (m_mapPluginInfo.find(info.id) != m_mapPluginInfo.end())
        {
            if (m_mapPluginInfo[info.id].author != info.author)
            {
                return 4; // 已存在同名插件但作者不同
            }
            
            if (m_mapPluginInfo[info.id].version >= info.version)
            {
                return 2; // 已存在同名插件且版本不低于当前版本
            }
        }

        m_mapPluginInfo[info.id] = info;
    }
    //接下来保存新信息到配置文件
    if (saveNow)
    {
        Save();
    }

    return 0; // 注册成功
}

bool PluginInfoManager::GetPluginInfo(const std::string& pluginID, PluginInfo &info)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (pluginID.empty())
    {
        return false;
    }
    if (m_mapPluginInfo.find(pluginID) == m_mapPluginInfo.end())
    {
        return false;
    }
    info = m_mapPluginInfo[pluginID];
    
    return true;
}

bool PluginInfoManager::SetPluginInfo(const PluginInfo & info, bool saveNow)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (info.id.empty())
        {
            return 3;
        }
        if (m_mapPluginInfo.find(info.id) == m_mapPluginInfo.end())
        {
            return false;
        }
        m_mapPluginInfo[info.id] = info;
    }
    //接下来保存新信息到配置文件
    if (saveNow)
    {
        Save();
    }
    
    return true;
}

bool PluginInfoManager::DeletePluginInfo(const std::string&  pluginID, bool saveNow)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (pluginID.empty())
        {
            return 3;
        }
        if (m_mapPluginInfo.find(pluginID) == m_mapPluginInfo.end())
        {
            return true; // 已经不存在了
        }
        m_mapPluginInfo.erase(pluginID);
    }
    //接下来保存已经删除插件的新信息到配置文件
    if (saveNow)
    {
        Save();
    }
    return true;
}

std::vector<PluginInfo> PluginInfoManager::GetPluginList()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<PluginInfo> pluginList;
    for (const auto& pair : m_mapPluginInfo) {
        pluginList.push_back(pair.second);
    }
    return pluginList;
}
