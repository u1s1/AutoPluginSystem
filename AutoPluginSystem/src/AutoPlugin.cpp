#include "AutoPluginManager.h"

int main()
{
    std::cout << "Registered APIs count: " << GetApiRegistry().size() << std::endl;
    PluginManager manager;
    if (manager.LoadAndStart("D:\\git\\AutoPluginSystem\\test\\Customer\\build\\Release\\MyPlugin.dll")) {
        std::cout << "Plugin loaded and started successfully!" << std::endl;
    } else {
        std::cout << "Failed to load or start plugin." << std::endl;
    }
    manager.Stop();
    std::cout << std::endl;

    // 程序结束前会自动卸载插件
    return 0;
}