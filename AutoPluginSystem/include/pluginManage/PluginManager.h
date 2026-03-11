#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <unordered_map>
#include <memory>
#include <mutex>
#include "PluginInstance.h"
#include "IPluginInstaller.h"

class PluginManager {
public:
    PluginManager() = default;
    PluginManager(std::unique_ptr<IPluginInstaller> installer, std::shared_ptr<PluginInfoManager> infoManager);
    ~PluginManager();

    //allCopy 参数表示是否将插件文件路径下全部文件复制到安装目录，false表示只复制路径指明的dll，默认为 true
    bool Install(const std::string& pluginPath, bool allCopy = true);

    void Uninstall(const std::string& pluginID);

    //按路径加载临时外部插件
    bool LoadByPath(const std::string& pluginID, const std::string& pluginPath);

    bool Load(const std::string& pluginID);

    bool Unload(const std::string& pluginID);
    //启动已安装插件
    bool Start(const std::string& pluginID);

    void Stop(const std::string& pluginID);

private:
    std::mutex m_mutex;
    std::unordered_map<std::string, std::shared_ptr<PluginInstance>> m_mapInstance;
    std::unique_ptr<IPluginInstaller> m_installer;
    std::shared_ptr<PluginInfoManager> m_infoManager;
};

#endif