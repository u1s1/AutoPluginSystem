#ifndef TEMP_SERVER_H
#define TEMP_SERVER_H

#include "AutoPluginABI.h"
#include "AutoPluginRegister.h"
#include "DriverManager.h"
#include "AutoPluginDriverAPI.h"
#include "IniOperator.h"
#include <iostream>

// 实现系统日志
void AutoPluginLog(const char* message) {
    std::cout << "[Host System] " << message << std::endl;

    std::shared_ptr<DriverInfoManager> infoManager = std::make_shared<DriverInfoManager>(std::make_unique<IniOperator>());
    std::shared_ptr<DriverLoader> loader = std::make_shared<DriverLoader>(infoManager,
        std::make_shared<SystemDriverRequireInfoManager>(std::make_unique<IniOperator>()));
    std::shared_ptr<DriverInstaller> installer = std::make_shared<DriverInstaller>(infoManager);
    DriverManager driverManager(loader, installer);
    if (driverManager.LoadByPath<DriverDispatchTable>( "AutoPluginDriver.dll", 1))
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

    if (driverManager.LoadByPath<DriverDispatchTable>("AutoPluginDriver.dll", 1)) {
        std::cout << "Driver loaded and started successfully!" << std::endl;
        // 这里可以通过 driverManager.m_driverTable 调用驱动函数
        int device = driverManager.GetDriverTable<DriverDispatchTable>()->CreateDevice();
        driverManager.GetDriverTable<DriverDispatchTable>()->DestroyDevice(device);
        driverManager.Uninstall<DriverDispatchTable>("AutoPluginDriver");
    } else {
        std::cout << "Failed to load or start driver." << std::endl;
    }
}
EXPORT_HOST_API(AutoPluginLog); // 原地注册！

#endif