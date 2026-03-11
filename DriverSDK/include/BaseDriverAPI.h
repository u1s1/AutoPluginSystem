#ifndef BASE_DRIVER_API_H
#define BASE_DRIVER_API_H
#include <stdint.h>
// --- 导出驱动表 ---
#if defined(_WIN32)
    #define DRIVER_EXPORT __declspec(dllexport)
#else
    #define DRIVER_EXPORT __attribute__((visibility("default")))
#endif

// 强制所有驱动表遵循的内存前缀契约
struct DriverHeader {
    uint32_t version;    // 驱动 API 版本号，用于向后兼容检查
    uint32_t tableSize;  // 结构体总字节数，防止新版宿主读取老版 DLL 导致内存越界
};

// 驱动 DLL 必须导出的唯一入口
typedef bool (*PFN_LoadDriver)(void* outTable);
typedef bool (*PFN_StopDriver)();
typedef bool (*PFN_UninstallDriver)();

#endif