#ifndef AUTO_PLUGIN_DRIVER_API_H
#define AUTO_PLUGIN_DRIVER_API_H
#include <stdint.h>
// --- 导出驱动表 ---
#if defined(_WIN32)
    #define DRIVER_EXPORT __declspec(dllexport)
#else
    #define DRIVER_EXPORT __attribute__((visibility("default")))
#endif

// 【驱动分发表】：纯函数指针，没有任何 C++ 类的开销
struct DriverDispatchTable {
    uint32_t version; // 驱动版本号，用于向后兼容
    
    // 设备管理 (低频)
    int (*CreateDevice)();
    void (*DestroyDevice)(int device);
};

// 驱动 DLL 必须导出的唯一入口
typedef bool (*PFN_LoadDriver)(DriverDispatchTable* outTable);
typedef bool (*PFN_StopDriver)();
typedef bool (*PFN_UninstallDriver)();

#endif