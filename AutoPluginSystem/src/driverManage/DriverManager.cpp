#include "DriverManager.h"

DriverManager::DriverManager(std::shared_ptr<DriverLoader> loader,
        std::shared_ptr<DriverInstaller> installer) :
    m_loader(loader),
    m_installer(installer)
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
