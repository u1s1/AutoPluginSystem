#include "InstallOperator.h"
#include <sys/stat.h>
#include <iostream>
#include "common.h"

int InstallOperator::InstallPlugin(const char *pluginPath,const PluginInfo& pluginInfo, bool allCopy)
{
    std::string installDir = GetExecutablePath() + "/\\" + pluginInfo.name;
    bool ret = false;
    if (allCopy)
    {
        ret = copy_directory_recursive(pluginPath, installDir);
    }
    else
    {
        ret = copy_single_file(pluginPath, installDir);
    }
    return ret ? 0 : -1;
}

void InstallOperator::UninstallPlugin(const char *pluginName)
{
}

bool InstallOperator::copy_directory_recursive(const fs::path &source, const fs::path &destination)
{
    try {
        if (!fs::exists(source) || !fs::is_directory(source)) {
            std::cerr << "source directory not exist" << std::endl;
            return false;
        }

        if (!fs::exists(destination)) {
            fs::create_directories(destination);
        }

        const auto copyOptions = fs::copy_options::recursive 
                               | fs::copy_options::overwrite_existing;

        fs::copy(source, destination, copyOptions);

        std::cout << "copy success!" << std::endl;

    } catch (fs::filesystem_error& e) {
        std::cerr << "copy error: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool InstallOperator::copy_single_file(const fs::path &sourceFile, const fs::path &targetPath)
{
    try {
        if (!fs::exists(sourceFile)) {
            std::cerr << "sourceFile  not exist: " << sourceFile << std::endl;
            return false;
        }

        fs::path destination = targetPath;
        if (!fs::exists(destination)) {
            fs::create_directories(destination);
        }
        
        if (fs::is_directory(targetPath)) {
            destination /= sourceFile.filename();
        }

        const auto options = fs::copy_options::overwrite_existing;

        if (fs::copy_file(sourceFile, destination, options)) {
            std::cout << "copy success: " << destination << std::endl;
        } else {
            std::cout << "copy error" << std::endl;
            return false;
        }

    } catch (const fs::filesystem_error& e) {
        std::cerr << "copy error:: " << e.what() << std::endl;
        return false;
    }
    return true;
}
