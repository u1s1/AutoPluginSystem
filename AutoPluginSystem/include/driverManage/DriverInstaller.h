#ifndef DRIVER_INSTALLER_H
#define DRIVER_INSTALLER_H
#include "DriverDef.h"
#include "DriverInfoManager.h"

class DriverInstaller
{
private:
    std::shared_ptr<DriverInfoManager> m_infoManager;
public:
    DriverInstaller(std::shared_ptr<DriverInfoManager> infoManager);
    ~DriverInstaller() = default;

    template <typename TTable>
    bool Install()
    {
        return true;
    }

    template <typename TTable>
    void Uninstall(const char *pluginID,
                   std::unordered_map<std::type_index, std::shared_ptr<DriverContext>> &drivers)
    {
        /*if (drivers[std::type_index(typeid(TTable))] != nullptr && 
            drivers[std::type_index(typeid(TTable))]->uninstallFunc != nullptr)
        {
            drivers[std::type_index(typeid(TTable))]->uninstallFunc();
        }
        drivers[std::type_index(typeid(TTable))].reset();*/
    }
};

#endif