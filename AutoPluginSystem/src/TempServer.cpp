#ifndef TEMP_SERVER_H
#define TEMP_SERVER_H

#include "../include/AutoPluginABI.h"
#include "../include/AutoPluginRegister.h"
#include "../include/AutoPluginDriverManager.h"
#include <iostream>

// 实现系统日志
void AutoPluginLog(const char* message) {
    std::cout << "[Host System] " << message << std::endl;

    PluginDriverManager driverManager;
    if (driverManager.LoadAndStart("AutoPluginDriver.dll")) {
        std::cout << "Driver loaded and started successfully!" << std::endl;
        // 这里可以通过 driverManager.m_driverTable 调用驱动函数
        int device = driverManager.m_driverTable.CreateDevice();
        driverManager.m_driverTable.DestroyDevice(device);
        driverManager.Unload();
    } else {
        std::cout << "Failed to load or start driver." << std::endl;
    }

    if (driverManager.LoadAndStart("AutoPluginDriver.dll")) {
        std::cout << "Driver loaded and started successfully!" << std::endl;
        // 这里可以通过 driverManager.m_driverTable 调用驱动函数
        int device = driverManager.m_driverTable.CreateDevice();
        driverManager.m_driverTable.DestroyDevice(device);
        driverManager.Uninstall();
    } else {
        std::cout << "Failed to load or start driver." << std::endl;
    }
}
EXPORT_HOST_API(AutoPluginLog); // 原地注册！

#endif