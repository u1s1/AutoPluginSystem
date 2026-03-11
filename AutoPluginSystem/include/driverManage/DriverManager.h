#ifndef DRIVERMANAGER_H
#define DRIVERMANAGER_H
#include "AutoPluginDriverAPI.h"
#include <string>
#include <iostream>
#include <mutex>
#include <memory>
#include "AutoPluginDef.h"

#define LOW_SUPPORT_VERSION 1

struct DriverContext {
    LibHandle handle = nullptr;
    DriverDispatchTable table{};
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

class DriverManager {
public:
    DriverManager();
    ~DriverManager();

    bool LoadAndStart(const char *pluginPath);

    void Unload();

    void Uninstall();

    std::shared_ptr<const DriverDispatchTable> GetDriverTable();

private:
    std::mutex m_mutex;
    std::shared_ptr<DriverContext> m_context;
};

#endif