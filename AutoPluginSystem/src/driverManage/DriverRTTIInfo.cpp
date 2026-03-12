#include "DriverRTTIInfo.h"

DriverRTTIInfo::DriverRTTIInfo()
{
    m_IdName.emplace(std::type_index(typeid(DriverDispatchTable)), "DriverDispatchTable");
    m_NameId.emplace("DriverDispatchTable", std::type_index(typeid(DriverDispatchTable)));
}

DriverRTTIInfo::~DriverRTTIInfo()
{
}

std::type_index DriverRTTIInfo::GetTypeID(const std::string &name)
{
    auto it = GetInstance().m_NameId.find(name);
    if (it == GetInstance().m_NameId.end()) 
    {
        return std::type_index(typeid(void));
    }
    return it->second;
}

std::vector<std::type_index> DriverRTTIInfo::GetAllTypeID()
{
    std::vector<std::type_index> vec;
    for (const auto &it : GetInstance().m_NameId)
    {
        vec.push_back(it.second);
    }
    return vec;
}

std::string DriverRTTIInfo::GetTypeName(std::type_index ID)
{
    auto it = GetInstance().m_IdName.find(ID);
    if (it == GetInstance().m_IdName.end()) 
    {
        return "";
    }
    return it->second;
}

std::vector<std::string> DriverRTTIInfo::GetAllTypeName()
{
    std::vector<std::string> vec;
    for (const auto &it : GetInstance().m_IdName)
    {
        vec.push_back(it.second);
    }
    return vec;
}
