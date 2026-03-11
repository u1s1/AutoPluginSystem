#include "PluginManager.h"
#include "InstallOperator.h"
#include "IniOperator.h"

int main()
{
    std::cout << "Registered APIs count: " << GetApiRegistry().size() << std::endl;
    std::shared_ptr<PluginInfoManager> infoManager = std::make_shared<PluginInfoManager>(std::make_unique<IniOperator>());
    PluginManager manager(std::make_unique<InstallOperator>(infoManager), infoManager);
    if (manager.Install("D:\\git\\AutoPluginSystem\\test\\Customer\\build\\Release\\MyPlugin.dll",false)) {
        std::cout << "Plugin Install successfully!" << std::endl;
    } else {
        std::cout << "Failed to Install plugin." << std::endl;
    }
    if (manager.Load("com.MyPlugin")) {
        std::cout << "Plugin  Load successfully!" << std::endl;
    } else {
        std::cout << "Failed to Load plugin." << std::endl;
    }
    if (manager.Start("com.MyPlugin")) {
        std::cout << "Plugin  started successfully!" << std::endl;
    } else {
        std::cout << "Failed to start plugin." << std::endl;
    }
    manager.Stop("com.MyPlugin");
    std::cout << std::endl;

    auto list = infoManager->GetPluginList();
    std::cout << "Installed plugins:" << std::endl;
    for (const auto& info : list) {
        std::cout << " - " << info.id << " (name: " << info.name<< ", version: " << info.version << ", author: " << info.author
        << ", description: " << info.description << ")" << std::endl;
    }

    // 程序结束前会自动卸载插件
    return 0;
}