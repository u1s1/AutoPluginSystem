#include "../include/AutoPluginDriverAPI.h"
#include "../include/Driver.h"

extern "C" DRIVER_EXPORT bool LoadDriver(DriverDispatchTable* outTable) {
    if (!outTable) return false;
    
    std::cout<<"LoadDriver called! Filling dispatch table..."<<std::endl;
    // 驱动把自己的函数地址塞进表里
    outTable->header.magicId = 1;
    outTable->header.tableSize = sizeof(DriverDispatchTable);
    outTable->header.version = 1;
    outTable->CreateDevice = CreateDevice;
    outTable->DestroyDevice = DestroyDevice;
    return true;
}

extern "C" DRIVER_EXPORT bool StopDriver() {
    std::cout<<"StopDriver called!"<<std::endl;
    return true;
}

extern "C" DRIVER_EXPORT bool UninstallDriver() {
    std::cout<<"UninstallDriver called!"<<std::endl;
    return true;
}