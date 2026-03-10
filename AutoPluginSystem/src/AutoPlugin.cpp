#include "PluginManager.h"

int main()
{
    std::cout << "Registered APIs count: " << GetApiRegistry().size() << std::endl;
    if (PluginManager::Install("D:\\git\\AutoPluginSystem\\test\\Customer\\build\\Release\\MyPlugin.dll",false)) {
        std::cout << "Plugin Install successfully!" << std::endl;
    } else {
        std::cout << "Failed to Install plugin." << std::endl;
    }
    if (PluginManager::Load("com.MyPlugin")) {
        std::cout << "Plugin  Load successfully!" << std::endl;
    } else {
        std::cout << "Failed to Load plugin." << std::endl;
    }
    if (PluginManager::Start("com.MyPlugin")) {
        std::cout << "Plugin  started successfully!" << std::endl;
    } else {
        std::cout << "Failed to start plugin." << std::endl;
    }
    PluginManager::Stop("com.MyPlugin");
    std::cout << std::endl;

    auto list = PluginInfoManager::GetPluginList();
    std::cout << "Installed plugins:" << std::endl;
    for (const auto& info : list) {
        std::cout << " - " << info.id << " (name: " << info.name<< ", version: " << info.version << ", author: " << info.author
        << ", description: " << info.description << ")" << std::endl;
    }

    // 程序结束前会自动卸载插件
    return 0;
}