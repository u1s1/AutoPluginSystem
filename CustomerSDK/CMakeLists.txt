cmake_minimum_required(VERSION 3.10)
project(AutoPluginAPI CXX)

# 1. 定义目标：编译为一个静态库 (STATIC)
add_library(AutoPluginAPI_static STATIC
    ./src/AutoPluginAPI.cpp
)

# 2. 核心防坑设置：开启 -fPIC (Position Independent Code)
# 【原理解析】因为这个静态库最终是要被第三方开发者链接进他们自己的动态库 (.dll / .so) 里的。
# 在 Linux/macOS 系统下，如果静态库没有开启 PIC，链接进动态库时必定会报 "relocation R_X86_64_32 against hidden symbol can not be used when making a shared object" 的致命错误。Windows 虽然默认支持，但显式开启是个好习惯。
set_target_properties(AutoPluginAPI_static PROPERTIES POSITION_INDEPENDENT_CODE ON)

# 3. 设置头文件包含路径 (Include Directories)
# 使用 PUBLIC 关键字是现代 CMake 的精髓：
# 它不仅告诉编译器编译 HostAPI_Static.cpp 时去哪里找头文件；
# 更神奇的是，未来任何人只要 target_link_libraries(MyPlugin AutoPluginAPI_static)，
# CMake 会自动把这两个 include 目录塞给 MyPlugin，开发者连 include 路径都不用自己配了！
target_include_directories(AutoPluginAPI_static
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 4. 强制使用至少 C++11 标准 (因为我们用到了局部静态变量等特性)
target_compile_features(AutoPluginAPI_static PUBLIC cxx_std_11)