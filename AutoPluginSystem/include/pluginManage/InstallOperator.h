#ifndef INSTALL_OPERATOR_H
#define INSTALL_OPERATOR_H
#include "AutoPluginDef.h"
#include <filesystem>
#include "PluginInfoManager.h"

namespace fs = std::filesystem;

class InstallOperator {
public:
    // 安装插件，返回值：0成功，1已存在同名插件，2创建安装目录失败
    static int InstallPlugin(const char *pluginPath, PluginInfo &pluginInfo, bool allCopy = true);

    // 卸载插件
    static void UninstallPlugin(const char *pluginID);

private:
    static bool copy_directory_recursive(const fs::path &source, const fs::path &destination);

    static bool copy_single_file(const fs::path &sourceFile, const fs::path &targetPath);
};

#endif