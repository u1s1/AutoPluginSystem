#ifndef AUTOPLUGINMANAGER_H
#define AUTOPLUGINMANAGER_H
#include "AutoPluginABI.h"
#include "AutoPluginRegister.h"
#include <string>
#include <iostream>
#include "AutoPluginDef.h"

class PluginManager {
public:
    PluginManager();
    ~PluginManager();

    bool LoadAndStart(const char *pluginPath);

    void Unload();

    void Uninstall();

private:
    LibHandle m_handle = nullptr;
    PFN_StopPlugin m_stopFunc = nullptr;
    PFN_UninstallPlugin m_uninstallFunc = nullptr;
};

#endif