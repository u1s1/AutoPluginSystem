#ifndef DRIVERINFOMANAGER_H
#define DRIVERINFOMANAGER_H

#include <unordered_map>
#include <typeindex>
#include <mutex>
#include <memory>
#include "common.h"
#include "DriverDef.h"
#include "IConfigParser.h"
#include "DriverRTTIInfo.h"

class DriverInfoManager{
private:
    void Init();
    void Save();
    std::mutex m_mutex;
    std::unordered_map<std::type_index, std::unordered_map<std::string, DriverInfo>> m_mapDriverInfo;
    std::unique_ptr<IConfigParser> m_configParser;

public:
    DriverInfoManager(std::unique_ptr<IConfigParser> configer);
    ~DriverInfoManager();
    //从插件路径下自带的配置文件解析出插件信息
    template <typename TTable>
    bool GetDriverInfoFromPath(const std::string &driverPath, DriverInfo &info)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string strDriverPath(driverPath);
        size_t lastSlash = strDriverPath.find_last_of("/\\");
        if (lastSlash == std::string::npos) {
            return false; // 无效路径
        }
        strDriverPath = strDriverPath.substr(0, lastSlash + 1); // 获取目录
        std::string configFilePath = strDriverPath + "driver_info.ini"; // 假设配置文件名固定
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
        info.moduleName = m_configParser->getValue(sections[0], "moduleName", "");
        info.name = m_configParser->getValue(sections[0], "name", "");
        info.description = m_configParser->getValue(sections[0], "description", "");
        info.version = m_configParser->getValue(sections[0], "version", "");
        info.author = m_configParser->getValue(sections[0], "author", "");
        info.running = false;

        return true;
    }

    // 注册插件信息并创建插件文件夹，返回值：0成功，1已存在同名插件，2创建安装目录失败
    template <typename TTable>
    int RegisterDriverInfo(const DriverInfo &info, bool saveNow = true)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (info.id.empty())
            {
                return 3; //待安装插件信息为空
            }
            auto it = m_mapDriverInfo.find(std::type_index(typeid(TTable)));
            if (it == m_mapDriverInfo.end())
            {
                return -1;
            }

            if (it->second.find(info.id) != it->second.end())
            {
                if (it->second.find(info.id)->second.author != info.author)
                {
                    return 4; // 已存在同名插件但作者不同
                }
                
                if (it->second.find(info.id)->second.version >= info.version)
                {
                    return 2; // 已存在同名插件且版本不低于当前版本
                }
            }
            
            m_mapDriverInfo[std::type_index(typeid(TTable))][info.id] = info;
        }
        //接下来保存新信息到配置文件
        if (saveNow)
        {
            Save();
        }

        return 0; // 注册成功
    }

    template <typename TTable>
    bool GetDriverInfo(const std::string& driverID, DriverInfo &info)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (driverID.empty())
        {
            return false;
        }
        auto it = m_mapDriverInfo.find(std::type_index(typeid(TTable)));
        if (it == m_mapDriverInfo.end())
        {
            return false;
        }

        if (it->second.find(info.id) == it->second.end())
        {
            return false;
        }
        info = it->second.find(info.id)->second;

        return true;
    }

    template <typename TTable>
    bool SetDriverInfo(const DriverInfo &info, bool saveNow = true)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_mapDriverInfo.find(std::type_index(typeid(TTable)));
        if (it == m_mapDriverInfo.end())
        {
            return false;
        }

        if (it->second.find(info.id) == it->second.end())
        {
            return false;
        }
        m_mapDriverInfo[std::type_index(typeid(TTable))][info.id] = info;

        return true;
    }

    template <typename TTable>
    bool DeleteDriverInfo(const std::string& driverID, bool saveNow = true)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (driverID.empty())
            {
                return false;
            }
            auto it = m_mapDriverInfo.find(std::type_index(typeid(TTable)));
            if (it == m_mapDriverInfo.end())
            {
                return false;
            }

            auto itDelete = it->second.find(driverID);
            if (itDelete == it->second.end())
            {
                return false;
            }
            m_mapDriverInfo.erase(itDelete);
        }
        //接下来保存已经删除插件的新信息到配置文件
        if (saveNow)
        {
            Save();
        }
        return true;
    }

    template <typename TTable>
    std::vector<DriverInfo> GetPluginList()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<DriverInfo> driverList;
        auto it = m_mapDriverInfo.find(std::type_index(typeid(TTable)));
        if (it == m_mapDriverInfo.end())
        {
            return false;
        }
        for (const auto& pair : it->second) {
            driverList.push_back(pair.second);
        }
        return driverList;
    }
};

#endif