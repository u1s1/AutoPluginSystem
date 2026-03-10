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
    if (manager.Start("MyPlugin")) {
        std::cout << "Plugin  started successfully!" << std::endl;
    } else {
        std::cout << "Failed to start plugin." << std::endl;
    }
    manager.Stop();
    std::cout << std::endl;

    // 程序结束前会自动卸载插件
    return 0;
}