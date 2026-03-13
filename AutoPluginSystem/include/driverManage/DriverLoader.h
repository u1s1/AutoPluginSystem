#ifndef DRIVER_LOADER_H
#define DRIVER_LOADER_H
#include "DriverDef.h"
#include "DriverInfoManager.h"
#include "SystemDriverRequireInfoManager.h"

class DriverLoader
{
private:
    std::shared_ptr<DriverInfoManager> m_infoManager;
    std::shared_ptr<SystemDriverRequireInfoManager> m_systemRequire;
public:
    DriverLoader(std::shared_ptr<DriverInfoManager> infoManager,
                 std::shared_ptr<SystemDriverRequireInfoManager> systemRequire);
    ~DriverLoader() = default;

    template <typename TTable>
    bool LoadByPath(const char *pluginPath, uint32_t minVersion,
                    std::unordered_map<std::type_index, std::shared_ptr<DriverContext>> &drivers,
                    std::shared_ptr<DriverInfo> info = nullptr)
    {
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
        if (pTable->header.version < minVersion) return false;
        if (pTable->header.tableSize > sizeof(TTable)) return false; // 防止驱动写爆内存

        // 将特定类型的智能指针向上转型为 void*，存入统一容器 (类型擦除)
        context->table = pTable;
        if (info)
        {
            context->info = info;
        }
        
        Unload<TTable>(drivers);
        drivers[std::type_index(typeid(TTable))] = std::move(context);
        return true;
    }

    template <typename TTable>
    bool Load(const char *pluginID, std::unordered_map<std::type_index, std::shared_ptr<DriverContext>> &drivers)
    {
        DriverInfo info;
        if (!m_infoManager->GetDriverInfo<TTable>(pluginID, info))
        {
            return false;
        }
        uint32_t minVersion = m_systemRequire->GetRequireInfo<TTable>().lowVersion;
        std::string pluginPath = GetExecutablePath() + "/\\" + info.id + "/\\" + info.name + ".dll";
        std::shared_ptr<DriverInfo> infoPtr = std::make_shared<DriverInfo>(info);
        return LoadByPath<TTable>(pluginPath.c_str(), minVersion, drivers, infoPtr);
    }

    template <typename TTable>
    void Unload(std::unordered_map<std::type_index, std::shared_ptr<DriverContext>> &drivers)
    {
        drivers[std::type_index(typeid(TTable))].reset();
    }
};

#endif