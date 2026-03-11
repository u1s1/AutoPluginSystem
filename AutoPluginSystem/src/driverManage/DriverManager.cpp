#include "DriverManager.h"

DriverManager::DriverManager()
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
