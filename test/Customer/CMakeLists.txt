cmake_minimum_required(VERSION 3.10)
project(MyCoolPlugin CXX)

# ---------------------------------------------------------
# 1. 定义目标：编译为动态加载模块 (MODULE)
# ---------------------------------------------------------
# 注意：在 CMake 中编译插件，强烈建议用 MODULE 而不是 SHARED！
# SHARED 是给别人链接用的动态库，而 MODULE 明确告诉 CMake：
# “这个库是用来在运行时被 dlopen/LoadLibrary 动态加载的，不需要生成导入库”。
add_library(MyPlugin MODULE
    AutoPluginCustomer.cpp
)

# ---------------------------------------------------------
# 2. 引入你的 SDK 头文件
# ---------------------------------------------------------
target_include_directories(MyPlugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ---------------------------------------------------------
# 3. 链接你的 SDK 静态库 (闭源实现)
# ---------------------------------------------------------
# 这里做一个简单的跨平台判断，告诉 CMake 去哪里找你的静态库文件
if(WIN32)
    set(SDK_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/AutoPluginAPI_static.lib")
else()
    set(SDK_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/libAutoPluginAPI_static.a")
endif()

# 将你的静态库狠狠地“揉进”这个动态库里
target_link_libraries(MyPlugin PRIVATE ${SDK_LIB_PATH})

# ---------------------------------------------------------
# 4. 现代 C++ 规范与符号隐藏 (高级优化)
# ---------------------------------------------------------
target_compile_features(MyPlugin PRIVATE cxx_std_11)

# 【核心防坑】：隐藏动态库内部的所有符号！
# 这样编译出来的 DLL/SO，不仅体积更小，而且只对外暴露你用 `PLUGIN_EXPORT` 宏标记的函数（如 SetupPluginAPI 和 StartPlugin）。
# 这能彻底杜绝不同插件之间发生“符号冲突”的惨剧。
set_target_properties(MyPlugin PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)