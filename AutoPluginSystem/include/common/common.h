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

std::string GetExecutablePath();

#endif