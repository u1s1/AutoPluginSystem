#include "DriverLoader.h"

DriverLoader::DriverLoader(std::shared_ptr<DriverInfoManager> infoManager, 
    std::shared_ptr<SystemDriverRequireInfoManager> systemRequire) :
    m_infoManager(infoManager),
    m_systemRequire(systemRequire)
{
}
