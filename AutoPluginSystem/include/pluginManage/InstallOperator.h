#ifndef INSTALL_OPERATOR_H
#define INSTALL_OPERATOR_H
#include "IPluginInstaller.h"

class InstallOperator : public IPluginInstaller {
public:
    InstallOperator(std::shared_ptr<PluginInfoManager> infoManager);
    ~InstallOperator();
    // 安装插件，返回值：0成功，1已存在同名插件，2创建安装目录失败
    int InstallPlugin(const std::string &pluginPath, PluginInfo &pluginInfo, bool allCopy = true) override;

    // 卸载插件
    void UninstallPlugin(const std::string &pluginID) override;

private:
    std::shared_ptr<PluginInfoManager> m_infoManager;
};

#endif