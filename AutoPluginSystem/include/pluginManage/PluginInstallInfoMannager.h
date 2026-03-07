#ifndef PLUGININSTALLINFOMANAGER_H
#define PLUGININSTALLINFOMANAGER_H

#include <map>
#include "AutoPluginDef.h"

class PluginInstallInfoMannager{
public:
    //从插件路径下自带的配置文件解析出插件信息
    static bool GetPluginInfoFromPath(const char *pluginPath, PluginInfo &info);

    // 注册插件信息并创建插件文件夹，返回值：0成功，1已存在同名插件，2创建安装目录失败
    static int RegisterPluginInfo(const PluginInfo &info);

    static bool GetPluginInfo(const char *pluginName, PluginInfo &info);

    static bool SetPluginInfo(const PluginInfo &info);

    static bool DeletePluginInfo(const char *pluginName);

private:
    PluginInstallInfoMannager();
    ~PluginInstallInfoMannager();

    void Init();

    static PluginInstallInfoMannager& getInstance()
    {
        PluginInstallInfoMannager instance;
        return instance;
    }

    std::map<std::string, PluginInfo> m_mapPluginInfo;
};

#endif