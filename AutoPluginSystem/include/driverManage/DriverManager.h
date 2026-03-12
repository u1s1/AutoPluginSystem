#ifndef DRIVERMANAGER_H
#define DRIVERMANAGER_H
#include <string>
#include <iostream>
#include <mutex>
#include <unordered_map>
#include <typeindex>
#include "DriverLoader.h"
#include "DriverInstaller.h"

class DriverManager {
private:
    std::mutex m_mutex;
    std::unordered_map<std::type_index, std::shared_ptr<DriverContext>> m_drivers;
    std::shared_ptr<DriverLoader> m_loader;
    std::shared_ptr<DriverInstaller> m_installer;

public:
    DriverManager(std::shared_ptr<DriverLoader> loader,
        std::shared_ptr<DriverInstaller> installer);
    ~DriverManager();

    // 模板化加载：宿主调用时必须指明期望的表类型 TTable
    template <typename TTable>
    bool LoadByPath(const char* pluginPath, uint32_t minVersion) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_loader->LoadByPath<TTable>(pluginPath, minVersion, m_drivers);
    }

    template <typename TTable>
    bool Load(const char* pluginID) 
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_loader->Load<TTable>(m_drivers);
    }

    template <typename TTable>
    void Unload()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_drivers[std::type_index(typeid(TTable))].reset();
    }

    void UnloadAll();

    template <typename TTable>
    void Uninstall(const char* pluginID)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_installer->Uninstall<TTable>(pluginID, m_drivers);
    }

    // 工作线程安全获取驱动表
    template <typename TTable>
    std::shared_ptr<const TTable> GetDriverTable() {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_drivers.find(std::type_index(typeid(TTable)));
        if (it == m_drivers.end() || !it->second->table) return nullptr;
        
        // 智能指针别名构造技术：返回特定类型的指针，但引用计数依然绑定在 context 上
        return std::shared_ptr<const TTable>(it->second, static_cast<const TTable*>(it->second->table.get()));
    }

};

#endif