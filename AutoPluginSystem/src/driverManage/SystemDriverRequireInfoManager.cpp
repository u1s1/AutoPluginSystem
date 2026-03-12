#include "SystemDriverRequireInfoManager.h"

SystemDriverRequireInfoManager::SystemDriverRequireInfoManager(std::unique_ptr<IConfigParser> configer):
    m_configer(std::move(configer))
{
    Init();
}

SystemDriverRequireInfoManager::~SystemDriverRequireInfoManager()
{
}

void SystemDriverRequireInfoManager::Init()
{
    std::string configFilePath = GetExecutablePath() + "/driver_require.ini"; // 假设配置文件路径固定
    if (!m_configer->load(configFilePath)) {
        return;
    }
    auto sections = m_configer->getAllSections();
    for (const auto& section : sections) {
        std::type_index index = DriverRTTIInfo::GetTypeID(section);
        SystemDriverInfo info;
        info.lowVersion = static_cast<uint32_t>(std::stoll(m_configer->getValue(section, "lowVersion", "")));
        m_info[index] = info;
    }
}
