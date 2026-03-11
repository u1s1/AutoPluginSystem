#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <unordered_map>
#include <memory>
#include <mutex>
#include "PluginInstance.h"

class PluginManager {
public:
    PluginManager();
    ~PluginManager();

    //allCopy 参数表示是否将插件文件路径下全部文件复制到安装目录，false表示只复制路径指明的dll，默认为 true
    bool Install(const char *pluginPath, bool allCopy = true);

    void Uninstall(const char *pluginID);

    //按路径加载临时外部插件
    bool LoadByPath(const char *pluginID, const char *pluginPath);

    bool Load(const char *pluginID);

    bool Unload(const char *pluginID);
    //启动已安装插件
    bool Start(const char *pluginID);

    void Stop(const char *pluginID);

private:
    std::mutex m_mutex;
    std::unordered_map<std::string, std::unique_ptr<PluginInstance>> m_mapInstance;
};

#endif