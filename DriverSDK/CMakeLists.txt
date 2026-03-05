cmake_minimum_required(VERSION 3.10)
project(AutoPluginDriver CXX)

# ---------------------------------------------------------
# 1. 定义目标：编译为动态加载模块 (MODULE)
# ---------------------------------------------------------
# 注意：在 CMake 中编译插件，强烈建议用 MODULE 而不是 SHARED！
# SHARED 是给别人链接用的动态库，而 MODULE 明确告诉 CMake：
# “这个库是用来在运行时被 dlopen/LoadLibrary 动态加载的，不需要生成导入库”。
file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")
add_library(AutoPluginDriver MODULE
    ${LIB_SOURCES}
)

# ---------------------------------------------------------
# 2. 引入你的 SDK 头文件
# ---------------------------------------------------------
target_include_directories(AutoPluginDriver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ---------------------------------------------------------
# 4. 现代 C++ 规范与符号隐藏 (高级优化)
# ---------------------------------------------------------
target_compile_features(AutoPluginDriver PRIVATE cxx_std_11)

# 【核心防坑】：隐藏动态库内部的所有符号！
# 这样编译出来的 DLL/SO，不仅体积更小，而且只对外暴露你用 `PLUGIN_EXPORT` 宏标记的函数（如 SetupPluginAPI 和 StartPlugin）。
# 这能彻底杜绝不同插件之间发生“符号冲突”的惨剧。
set_target_properties(AutoPluginDriver PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)