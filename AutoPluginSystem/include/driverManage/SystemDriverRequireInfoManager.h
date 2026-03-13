#ifndef SYSTEM_DRIVER_REQUIRE_INFO_MANAGER_H
#define SYSTEM_DRIVER_REQUIRE_INFO_MANAGER_H

#include <memory>
#include <unordered_map>
#include <typeindex>
#include "IConfigParser.h"
#include <DriverRTTIInfo.h>
#include "common.h"

struct SystemDriverInfo
{
    uint32_t lowVersion;
};

//管理系统所有模块驱动的要求，比如最低版本等
class SystemDriverRequireInfoManager
{
private:
    std::unordered_map<std::type_index, SystemDriverInfo> m_info;
    std::unique_ptr<IConfigParser> m_configer;

    void Init();
public:
    SystemDriverRequireInfoManager(std::unique_ptr<IConfigParser> configer);
    ~SystemDriverRequireInfoManager();

    template <typename TTable>
    const SystemDriverInfo GetRequireInfo()
    {
        auto it = m_info.find(std::type_index(typeid(TTable)));
        if (it == m_info.end()) 
        {
            return {};
        }
        return it->second;
    }
};

#endif