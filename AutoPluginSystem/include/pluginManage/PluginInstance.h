#ifndef PLUGININSTANCE_H
#define PLUGININSTANCE_H

#include <string>
#include <iostream>
#include "AutoPluginABI.h"
#include "AutoPluginRegister.h"
#include "AutoPluginDef.h"
#include "PluginInfoManager.h"

class PluginInstance {
public:
    PluginInstance();
    ~PluginInstance();

    //按路径加载临时外部插件
    bool LoadByPath(const char *pluginPath);

    bool Load(const char *pluginId);

    void Unload();
    //启动已安装插件
    bool Start();

    void Stop();
private:
    LibHandle m_handle = nullptr;
    PFN_StartPlugin m_startFunc = nullptr;
    PFN_StopPlugin m_stopFunc = nullptr;
    PluginInfo m_pluginInfo;
};

#endif