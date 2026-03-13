#ifndef DRIVER_INSTALLER_H
#define DRIVER_INSTALLER_H
#include "DriverDef.h"
#include "DriverInfoManager.h"

class DriverInstaller
{
private:
    std::shared_ptr<DriverInfoManager> m_infoManager;
public:
    DriverInstaller(std::shared_ptr<DriverInfoManager> infoManager);
    ~DriverInstaller() = default;

    template <typename TTable>
    bool Install(const char *pluginPath,
                 bool allCopy = true)
    {
        DriverInfo info;
        if(!m_infoManager->GetDriverInfoFromPath<TTable>(pluginPath, info))
        {
            std::cout << "analysis failed" << std::endl;
            return false; // 解析失败
        }
        
        if (m_infoManager->RegisterDriverInfo<TTable>(info) != 0)
        {
            std::cout << "register failed" << std::endl;
            return false; // 注册失败
        }

        std::string installDir = GetExecutablePath() + "/\\" + info.id;
        bool ret = false;
        if (allCopy)
        {
            std::string strPluginPath(pluginPath);
            size_t lastSlash = strPluginPath.find_last_of("/\\");
            if (lastSlash == std::string::npos) {
                return false; // 无效路径
            }
            strPluginPath = strPluginPath.substr(0, lastSlash); // 获取目录
            std::cout<< "Plugin directory: " << strPluginPath << std::endl;
            ret = Copy_directory_recursive(strPluginPath, installDir);
        }
        else
        {
            ret = Copy_single_file(pluginPath, installDir);
        }
        return ret;
    }

    template <typename TTable>
    void Uninstall(const char *pluginID,
                   std::unordered_map<std::type_index, std::shared_ptr<DriverContext>> &drivers)
    {
        if (drivers[std::type_index(typeid(TTable))] != nullptr && 
            drivers[std::type_index(typeid(TTable))]->info->id == std::string(pluginID) && 
            drivers[std::type_index(typeid(TTable))]->uninstallFunc != nullptr)
        {
            drivers[std::type_index(typeid(TTable))]->uninstallFunc();
            drivers[std::type_index(typeid(TTable))].reset();
        }
        
        m_infoManager->DeleteDriverInfo<TTable>(pluginID);
        fs::path dirToRemove = GetExecutablePath() + "/\\" + pluginID;
        try {
            // remove_all 删除目录及其所有内容
            if (fs::exists(dirToRemove) && fs::is_directory(dirToRemove)) {
                fs::remove_all(dirToRemove);
                std::cout << "delete directory success!" << std::endl;
            } else {
                std::cout << "directory not exist!" << std::endl;
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "delete directory error: " << e.what() << std::endl;
        }
    }
};

#endif