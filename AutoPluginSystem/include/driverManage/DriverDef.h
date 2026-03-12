#ifndef DRIVER_DEF_H
#define DRIVER_DEF_H
#include <string>
#include "BaseDriverAPI.h"
#include <memory>

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

struct DriverInfo{
    std::string moduleName;
    std::string id;
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    bool running = false;
    bool defaultLoad = false;
};

struct DriverContext {
    LibHandle handle = nullptr;
    std::shared_ptr<void> table;
    PFN_StopDriver stopFunc = nullptr;
    PFN_UninstallDriver uninstallFunc = nullptr;

    // 析构函数：当最后一个 shared_ptr 被销毁时触发
    ~DriverContext() {
        if (handle) {
            if (stopFunc) {
                stopFunc(); // 停止业务逻辑
            }
            CLOSE_LIB(handle); // 安全卸载 DLL
        }
    }
};

#endif