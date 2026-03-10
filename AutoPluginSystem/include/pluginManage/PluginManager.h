#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <unordered_map>
#include <memory>
#include "PluginInstance.h"

class PluginManager {
public:

    //allCopy 参数表示是否将插件文件路径下全部文件复制到安装目录，false表示只复制路径指明的dll，默认为 true
    static bool Install(const char *pluginPath, bool allCopy = true);

    static void Uninstall(const char *pluginID);

    //按路径加载临时外部插件
    static bool LoadByPath(const char *pluginID, const char *pluginPath);

    static bool Load(const char *pluginID);

    static bool Unload(const char *pluginID);
    //启动已安装插件
    static bool Start(const char *pluginID);

    static void Stop(const char *pluginID);

private:
    std::unordered_map<std::string, std::unique_ptr<PluginInstance>> m_mapInstance;
    
    PluginManager();
    ~PluginManager();

    static PluginManager& getInstance()
    {
        static PluginManager instance;
        return instance;
    }
};

#endif