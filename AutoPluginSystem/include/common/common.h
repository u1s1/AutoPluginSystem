#ifndef COMMON_H
#define COMMON_H

#include <string>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define MKDIR(a) _mkdir(a)
#elif __APPLE__
#include <mach-o/dyld.h>
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(a) mkdir(a, 0755)
#else
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(a) mkdir(a, 0755)
#endif

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::string GetExecutablePath();

bool Copy_directory_recursive(const fs::path &source, const fs::path &destination);

bool Copy_single_file(const fs::path &sourceFile, const fs::path &targetPath);

#endif