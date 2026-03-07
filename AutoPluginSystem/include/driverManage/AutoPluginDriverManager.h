#ifndef AUTOPLUGINDRIVERMANAGER_H
#define AUTOPLUGINDRIVERMANAGER_H
#include "AutoPluginDriverAPI.h"
#include <string>
#include <iostream>
#include "AutoPluginDef.h"

class PluginDriverManager {
public:
    DriverDispatchTable m_driverTable;

    PluginDriverManager();
    ~PluginDriverManager();

    bool LoadAndStart(const char *pluginPath);

    void Unload();

    void Uninstall();

private:
    LibHandle m_handle = nullptr;
    PFN_StopDriver m_stopFunc = nullptr;
    PFN_UninstallDriver m_uninstallFunc = nullptr;
};

#endif