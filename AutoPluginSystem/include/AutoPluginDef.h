#ifndef AUTO_PLUGIN_DEF_H
#define AUTO_PLUGIN_DEF_H
#include <string>

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

struct PluginInfo{
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    bool running = false;
};
#endif