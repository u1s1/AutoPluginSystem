#ifndef AUTOPLUGINMANAGER_H
#define AUTOPLUGINMANAGER_H
#include "AutoPluginABI.h"
#include "AutoPluginRegister.h"
#include <string>
#include <iostream>

#if defined(_WIN32)
    #include <windows.h>
    typedef HMODULE LibHandle;
    #define LOAD_LIB(path) LoadLibraryA(path)
    #define GET_FUNC GetProcAddress
    #define CLOSE_LIB FreeLibrary
#else
    #include <dlfcn.h>
    typedef void* LibHandle;
    #define LOAD_LIB(path) dlopen(path, RTLD_LAZY)
    #define GET_FUNC dlsym
    #define CLOSE_LIB dlclose
#endif

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