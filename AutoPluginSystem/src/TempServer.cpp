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
    if (driverManager.LoadAndStart<DriverDispatchTable>( "AutoPluginDriver.dll", 1))
    {
        std::cout << "Driver loaded and started successfully!" << std::endl;
        // 这里可以通过 driverManager.m_driverTable 调用驱动函数
        int device = driverManager.GetDriverTable<DriverDispatchTable>()->CreateDevice();
        driverManager.GetDriverTable<DriverDispatchTable>()->DestroyDevice(device);
        driverManager.Unload<DriverDispatchTable>();
    }
    else
    {
        std::cout << "Failed to load or start driver." << std::endl;
    }

    if (driverManager.LoadAndStart<DriverDispatchTable>("AutoPluginDriver.dll", 1)) {
        std::cout << "Driver loaded and started successfully!" << std::endl;
        // 这里可以通过 driverManager.m_driverTable 调用驱动函数
        int device = driverManager.GetDriverTable<DriverDispatchTable>()->CreateDevice();
        driverManager.GetDriverTable<DriverDispatchTable>()->DestroyDevice(device);
        driverManager.Uninstall<DriverDispatchTable>();
    } else {
        std::cout << "Failed to load or start driver." << std::endl;
    }
}
EXPORT_HOST_API(AutoPluginLog); // 原地注册！

#endif