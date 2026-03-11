#ifndef TEMP_SERVER_H
#define TEMP_SERVER_H

#include "AutoPluginABI.h"
#include "AutoPluginRegister.h"
#include "DriverManager.h"
#include "AutoPluginDriverAPI.h"
#include <iostream>

// 实现系统日志
void AutoPluginLog(const char* message) {
    std::cout << "[Host System] " << message << std::endl;

    DriverManager driverManager;
    if (driverManager.LoadAndStart<DriverDispatchTable>(1, "AutoPluginDriver.dll", 1))
    {
        std::cout << "Driver loaded and started successfully!" << std::endl;
        // 这里可以通过 driverManager.m_driverTable 调用驱动函数
        int device = driverManager.GetDriverTable<DriverDispatchTable>(1)->CreateDevice();
        driverManager.GetDriverTable<DriverDispatchTable>(1)->DestroyDevice(device);
        driverManager.Unload(1);
    }
    else
    {
        std::cout << "Failed to load or start driver." << std::endl;
    }

    if (driverManager.LoadAndStart<DriverDispatchTable>(1, "AutoPluginDriver.dll", 1)) {
        std::cout << "Driver loaded and started successfully!" << std::endl;
        // 这里可以通过 driverManager.m_driverTable 调用驱动函数
        int device = driverManager.GetDriverTable<DriverDispatchTable>(1)->CreateDevice();
        driverManager.GetDriverTable<DriverDispatchTable>(1)->DestroyDevice(device);
        driverManager.Uninstall(1);
    } else {
        std::cout << "Failed to load or start driver." << std::endl;
    }
}
EXPORT_HOST_API(AutoPluginLog); // 原地注册！

#endif