#ifndef TEMP_SERVER_H
#define TEMP_SERVER_H

#include "../include/AutoPluginABI.h"
#include "../include/AutoPluginRegister.h"
#include <iostream>

// 实现系统日志
void AutoPluginLog(LogLevel level, const char* message) {
    std::cout << "[Host System] " << message << std::endl;
}
EXPORT_HOST_API(AutoPluginLog); // 原地注册！

#endif