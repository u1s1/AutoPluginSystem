#ifndef PLUGININFOMANAGER_H
#define PLUGININFOMANAGER_H

#include <unordered_map>
#include "AutoPluginDef.h"

class PluginInfoManager{
public:
    //从插件路径下自带的配置文件解析出插件信息
    static bool GetPluginInfoFromPath(const std::string& pluginPath, PluginInfo &info);

    // 注册插件信息并创建插件文件夹，返回值：0成功，1已存在同名插件，2创建安装目录失败
    static int RegisterPluginInfo(const PluginInfo &info);

    static bool GetPluginInfo(const std::string& pluginID, PluginInfo &info);

    static bool SetPluginInfo(const PluginInfo &info);

    static bool DeletePluginInfo(const std::string& pluginID);

    static std::vector<PluginInfo> GetPluginList();

private:
    PluginInfoManager();
    ~PluginInfoManager();

    void Init();

    void Save();

    static PluginInfoManager& getInstance()
    {
        static PluginInfoManager instance;
        return instance;
    }

    std::unordered_map<std::string, PluginInfo> m_mapPluginInfo;
};

#endif