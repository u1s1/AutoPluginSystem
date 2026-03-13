#include "common.h"

std::string GetExecutablePath()
 {
    std::string path;
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    path = buffer;
#elif __APPLE__
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0) {
        path = buffer;
    }
#else
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        path = buffer;
    }
#endif
    // 获取目录 (去掉文件名)
    size_t lastSlash = path.find_last_of("/\\");
    return (lastSlash != std::string::npos) ? path.substr(0, lastSlash) : "";
}

bool Copy_directory_recursive(const fs::path &source, const fs::path &destination)
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

bool Copy_single_file(const fs::path &sourceFile, const fs::path &targetPath)
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
