#ifndef PLUGININFOMANAGER_H
#define PLUGININFOMANAGER_H

#include <unordered_map>
#include <mutex>
#include <memory>
#include "AutoPluginDef.h"
#include "IConfigParser.h"

class PluginInfoManager{
public:
    PluginInfoManager(std::unique_ptr<IConfigParser> configer);
    ~PluginInfoManager();
    //从插件路径下自带的配置文件解析出插件信息
    bool GetPluginInfoFromPath(const std::string &pluginPath, PluginInfo &info);

    // 注册插件信息并创建插件文件夹，返回值：0成功，1已存在同名插件，2创建安装目录失败
    int RegisterPluginInfo(const PluginInfo &info, bool saveNow = true);

    bool GetPluginInfo(const std::string& pluginID, PluginInfo &info);

    bool SetPluginInfo(const PluginInfo &info, bool saveNow = true);

    bool DeletePluginInfo(const std::string& pluginID, bool saveNow = true);

    std::vector<PluginInfo> GetPluginList();

private:

    void Init();

    void Save();

    std::mutex m_mutex;
    std::unordered_map<std::string, PluginInfo> m_mapPluginInfo;
    std::unique_ptr<IConfigParser> m_configParser;
};

#endif