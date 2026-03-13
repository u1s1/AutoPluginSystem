#include "InstallOperator.h"
#include <sys/stat.h>
#include <iostream>
#include "common.h"

InstallOperator::InstallOperator(std::shared_ptr<PluginInfoManager> infoManager) : 
    m_infoManager(infoManager)
{
}

InstallOperator::~InstallOperator()
{
}

int InstallOperator::InstallPlugin(const std::string &pluginPath, PluginInfo &pluginInfo, bool allCopy)
{
    if(!m_infoManager->GetPluginInfoFromPath(pluginPath, pluginInfo))
    {
        std::cout << "analysis failed" << std::endl;
        return 2; // 解析失败
    }
    
    if (m_infoManager->RegisterPluginInfo(pluginInfo) != 0)
    {
        std::cout << "register failed" << std::endl;
        return 3; // 注册失败
    }

    std::string installDir = GetExecutablePath() + "/\\" + pluginInfo.id;
    bool ret = false;
    if (allCopy)
    {
        std::string strPluginPath(pluginPath);
        size_t lastSlash = strPluginPath.find_last_of("/\\");
        if (lastSlash == std::string::npos) {
            return -1; // 无效路径
        }
        strPluginPath = strPluginPath.substr(0, lastSlash); // 获取目录
        std::cout<< "Plugin directory: " << strPluginPath << std::endl;
        ret = Copy_directory_recursive(strPluginPath, installDir);
    }
    else
    {
        ret = Copy_single_file(pluginPath, installDir);
    }
    return ret ? 0 : -1;
}

void InstallOperator::UninstallPlugin(const std::string& pluginID)
{
    m_infoManager->DeletePluginInfo(pluginID);
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
