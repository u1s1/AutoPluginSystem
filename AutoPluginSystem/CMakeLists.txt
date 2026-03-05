cmake_minimum_required(VERSION 3.10)
project(MySystemHost CXX)

# 1. 强制使用现代 C++ 标准 (C++11 或更高，因为用到了 magic statics 和 unordered_map)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. 收集宿主的所有源文件
# 提示：如果你的源文件很多，可以使用 file(GLOB SOURCES "src/*.cpp")，但直接列出更利于维护
file(GLOB_RECURSE HOST_SOURCES "src/*.cpp")

# 3. 定义目标：编译为可执行文件 (Executable)
add_executable(HostMain ${HOST_SOURCES})

# 4. 设置头文件包含路径
# 让 src 下的 cpp 文件能够 #include "../Shared/HostABI.h"
target_include_directories(HostMain PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ---------------------------------------------------------
# 5. 【核心防坑】：链接操作系统的动态加载库
# ---------------------------------------------------------
# 在 Linux/macOS 下，dlopen, dlsym 等函数实现在 libdl 库中。
# 如果不链接它，编译时会报 "undefined reference to dlopen" 的致命错误。
# CMake 提供了一个非常贴心的内置跨平台变量：${CMAKE_DL_LIBS}
# 它在 Linux 下会自动解析为 "dl"，在 Windows 下会自动为空（因为 Windows 的 LoadLibrary 默认自带）。
target_link_libraries(HostMain PRIVATE ${CMAKE_DL_LIBS})

# ---------------------------------------------------------
# 6. 高级特性（可选但推荐）：支持内部多线程
# ---------------------------------------------------------
# 如果宿主未来会开子线程加载插件，或者使用共享内存/线程池，提前链接 pthread 是好习惯。
find_package(Threads REQUIRED)
target_link_libraries(HostMain PRIVATE Threads::Threads)