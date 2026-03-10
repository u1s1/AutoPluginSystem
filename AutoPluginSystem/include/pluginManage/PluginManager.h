#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H
#include "AutoPluginABI.h"
#include "AutoPluginRegister.h"
#include <iostream>
#include "AutoPluginDef.h"
#include "PluginInfoMannager.h"
#include "ThreadPoolLockFree.h"

class PluginManager {
public:
    PluginManager();
    ~PluginManager();

    //allCopy 参数表示是否将插件文件路径下全部文件复制到安装目录，false表示只复制路径指明的dll，默认为 true
    bool Install(const char *pluginPath, bool allCopy = true);

    void Uninstall();

    //按路径加载临时外部插件
    bool LoadByPath(const char *pluginPath);

    bool Load(const char *pluginName);
    //启动已安装插件
    bool Start();

    void Stop();

private:
    LibHandle m_handle = nullptr;
    PFN_StartPlugin m_startFunc = nullptr;
    PFN_StopPlugin m_stopFunc = nullptr;
    PFN_UninstallPlugin m_uninstallFunc = nullptr;
    ThreadPool m_threadPool{1}; // 线程池实例
    PluginInfo m_pluginInfo;
};

#endif