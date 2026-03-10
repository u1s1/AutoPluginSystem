#ifndef DRIVERMANAGER_H
#define DRIVERMANAGER_H
#include "AutoPluginDriverAPI.h"
#include <string>
#include <iostream>
#include "AutoPluginDef.h"

class DriverManager {
public:
    DriverDispatchTable m_driverTable;

    DriverManager();
    ~DriverManager();

    bool LoadAndStart(const char *pluginPath);

    void Unload();

    void Uninstall();

private:
    LibHandle m_handle = nullptr;
    PFN_StopDriver m_stopFunc = nullptr;
    PFN_UninstallDriver m_uninstallFunc = nullptr;
};

#endif