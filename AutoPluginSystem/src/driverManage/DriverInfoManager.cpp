#include "DriverInfoManager.h"

DriverInfoManager::DriverInfoManager(std::unique_ptr<IConfigParser> configer):
    m_configParser(std::move(configer))
{
    Init();
}

DriverInfoManager::~DriverInfoManager()
{
    Save();
}

void DriverInfoManager::Init()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    //解析配置文件，填充 m_mapPluginInfo
    m_mapDriverInfo.clear();

    std::string configFilePath = GetExecutablePath() + "/driver_info.ini"; // 假设配置文件路径固定
    if (!m_configParser->load(configFilePath)) {
        return;
    }
    auto sections = m_configParser->getAllSections();
    for (const auto& section : sections) {
        DriverInfo info;
        info.id = section;
        info.moduleName = m_configParser->getValue(section, "moduleName", "");
        info.name = m_configParser->getValue(section, "name", "");
        info.description = m_configParser->getValue(section, "description", "");
        info.version = m_configParser->getValue(section, "version", "");
        info.author = m_configParser->getValue(section, "author", "");
        info.defaultLoad = m_configParser->getValue(section, "defaultLoad", "") == "true";
        info.running = false;
        m_mapDriverInfo[DriverRTTIInfo::GetTypeID(info.moduleName)][info.id] = info;
    }
}

void DriverInfoManager::Save()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_configParser->load(GetExecutablePath() + "/driver_info.ini"))
    {
        return;
    }
    for (const auto& pair : m_mapDriverInfo) {
        for(const auto& subPair : pair.second)
        {
            const DriverInfo& info = subPair.second;
            // 业务层自己负责将 PluginInfo 转换为通用的 Map
            std::map<std::string, std::string> data;
            data["moduleName"] =  info.moduleName;
            data["name"] = info.name;
            data["version"] = info.version;
            data["author"] = info.author;
            data["description"] = info.description;
            data["defaultLoad"] = info.defaultLoad ? "true" : "false";
            // 调用底层通用接口
            m_configParser->setSectionData(info.id, data);
        }
    }
    m_configParser->save();
}