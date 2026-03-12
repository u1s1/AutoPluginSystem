#include "DriverManager.h"

DriverManager::DriverManager(std::shared_ptr<DriverInfoManager> infoManager,
                  std::shared_ptr<SystemDriverRequireInfoManager> systemRequire) :
    m_infoManager(infoManager),
    m_systemRequire(systemRequire)
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
