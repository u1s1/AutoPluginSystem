#include "PluginManager.h"

int main()
{
    std::cout << "Registered APIs count: " << GetApiRegistry().size() << std::endl;
    PluginManager manager;
    if (manager.Install("D:\\git\\AutoPluginSystem\\test\\Customer\\build\\Release\\MyPlugin.dll")) {
        std::cout << "Plugin loaded and started successfully!" << std::endl;
    } else {
        std::cout << "Failed to load or start plugin." << std::endl;
    }
    manager.Load("MyPlugin");
    if (manager.Start()) {
        std::cout << "Plugin  started successfully!" << std::endl;
    } else {
        std::cout << "Failed to start plugin." << std::endl;
    }
    manager.Stop();
    std::cout << std::endl;

    std::vector<PluginInfo> list = PluginInfoMannager::GetPluginList();
    std::cout << "Installed plugins:" << std::endl;
    for (const auto& info : list) {
        std::cout << " - " << info.name << " (version: " << info.version << ", author: " << info.author
        << ", description: " << info.description << ")" << std::endl;
    }

    // 程序结束前会自动卸载插件
    return 0;
}