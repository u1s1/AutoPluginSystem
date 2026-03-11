#ifndef DRIVERMANAGER_H
#define DRIVERMANAGER_H
#include "BaseDriverAPI.h"
#include <string>
#include <iostream>
#include <mutex>
#include <memory>
#include <unordered_map>
#include "AutoPluginDef.h"

#define LOW_SUPPORT_VERSION 1

struct DriverContext {
    LibHandle handle = nullptr;
    std::shared_ptr<void> table;
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
private:
    std::mutex m_mutex;
    std::unordered_map<uint32_t, std::shared_ptr<DriverContext>> m_drivers;

public:
    DriverManager();
    ~DriverManager();

    // 模板化加载：宿主调用时必须指明期望的表类型 TTable
    template <typename TTable>
    bool LoadAndStart(const uint32_t& driverId, const char* pluginPath, uint32_t minVersion) {
        auto context = std::make_shared<DriverContext>();
        context->handle = LOAD_LIB(pluginPath);
        if (!context->handle) return false;

        // 宿主负责分配具体类型的内存块
        auto pTable = std::make_shared<TTable>(); 
        
        auto loadDriver = (PFN_LoadDriver)GET_FUNC(context->handle, "LoadDriver");
        context->stopFunc = (PFN_StopDriver)GET_FUNC(context->handle, "StopDriver");
        context->uninstallFunc = (PFN_UninstallDriver)GET_FUNC(context->handle, "UninstallDriver");

        if (!loadDriver || !context->stopFunc || !context->uninstallFunc) return false;

        // 驱动内部会将 void* 强转为它自己的表类型并填充
        if (!loadDriver(pTable.get())) return false;

        // 核心安全层：跨 DLL 的 API 版本控制与越界校验
        if (pTable->header.magicId != driverId) return false;
        if (pTable->header.version < minVersion) return false;
        if (pTable->header.tableSize > sizeof(TTable)) return false; // 防止驱动写爆内存

        // 将特定类型的智能指针向上转型为 void*，存入统一容器 (类型擦除)
        context->table = pTable;
        Unload(driverId);
        std::lock_guard<std::mutex> lock(m_mutex);
        m_drivers[driverId] = std::move(context);
        return true;
    }

    void Unload(const uint32_t& driverId);

    void UnloadAll();

    void Uninstall(const uint32_t& driverId);

    // 工作线程安全获取驱动表
    template <typename TTable>
    std::shared_ptr<const TTable> GetDriverTable(const uint32_t& driverId) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_drivers.find(driverId);
        if (it == m_drivers.end() || !it->second->table) return nullptr;
        
        // 智能指针别名构造技术：返回特定类型的指针，但引用计数依然绑定在 context 上
        return std::shared_ptr<const TTable>(it->second, static_cast<const TTable*>(it->second->table.get()));
    }

};

#endif