#include "DriverManager.h"

DriverManager::DriverManager(std::shared_ptr<DriverLoader> loader) :
    m_loader(loader)
{
}

DriverManager::~DriverManager()
{
    UnloadAll();
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
