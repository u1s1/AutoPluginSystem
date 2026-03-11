#ifndef IPLUGININSTALLER_H
#define IPLUGININSTALLER_H

#include "AutoPluginDef.h"
#include "PluginInfoManager.h"

class IPluginInstaller {
public:
    virtual ~IPluginInstaller() = default;

    virtual int InstallPlugin(const std::string &pluginPath, PluginInfo &pluginInfo, bool allCopy = true) = 0;
    virtual void UninstallPlugin(const std::string &pluginID) = 0;
};

#endif // IPLUGININSTALLER_H