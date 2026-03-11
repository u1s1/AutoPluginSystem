#ifndef AUTO_PLUGIN_DRIVER_API_H
#define AUTO_PLUGIN_DRIVER_API_H
#include "BaseDriverAPI.h"

// 【驱动分发表】：纯函数指针，没有任何 C++ 类的开销
struct DriverDispatchTable {
    DriverHeader header;
    
    // 设备管理 (低频)
    int (*CreateDevice)();
    void (*DestroyDevice)(int device);
};

#endif