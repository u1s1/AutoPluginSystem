#include "DriverManager.h"

DriverManager::DriverManager()
{
}

DriverManager::~DriverManager()
{
    UnloadAll();
}

void DriverManager::Unload(const uint32_t& driverId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_drivers[driverId].reset();
}

void DriverManager::UnloadAll()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto &it : m_drivers)
    {
        if (it.second != nullptr)
        {
            it.second.reset();
        }
    }
    
}

void DriverManager::Uninstall(const uint32_t& driverId)
{
    std::shared_ptr<DriverContext> tempContext = nullptr;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        tempContext = m_drivers[driverId];
        m_drivers[driverId].reset();
    }
    if (tempContext != nullptr && tempContext->uninstallFunc != nullptr)
    {
        tempContext->uninstallFunc();
    }
    tempContext.reset();

    //下面执行删除驱动文件及收尾操作
    /* */
}
