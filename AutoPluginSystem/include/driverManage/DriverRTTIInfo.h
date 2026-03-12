#ifndef DRIVER_RTTI_INFO_H
#define DRIVER_RTTI_INFO_H

#include <unordered_map>
#include <typeindex>
#include <string>
#include <vector>
#include "AutoPluginDriverAPI.h"

class DriverRTTIInfo
{
private:
    std::unordered_map<std::type_index, std::string> m_IdName;
    std::unordered_map<std::string, std::type_index> m_NameId;

    DriverRTTIInfo();
    ~DriverRTTIInfo();

    static DriverRTTIInfo& GetInstance()
    {
        static DriverRTTIInfo instance;
        return instance;
    }
public:
    static std::type_index GetTypeID(const std::string &name);

    static std::vector<std::type_index> GetAllTypeID();

    static std::string GetTypeName(std::type_index ID);

    static std::vector<std::string> GetAllTypeName();

    template <typename TTable>
    static std::string GetTypeName()
    {
        return GetTypeName(std::type_index(typeid(TTable)));
    }
};

#endif