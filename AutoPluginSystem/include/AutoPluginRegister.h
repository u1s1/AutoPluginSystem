#ifndef AUTOPLUGINREGISTER_H
#define AUTOPLUGINREGISTER_H
#include <unordered_map>
#include <string>

// 1. 全局唯一的注册表 (局部静态变量，避免 SIOF 顺序问题)
inline std::unordered_map<std::string, void*>& GetApiRegistry() {
    static std::unordered_map<std::string, void*> registry;
    return registry;
}

// 2. 注册机结构体
struct ApiRegistrar {
    ApiRegistrar(const char* apiName, void* funcPtr) {
        GetApiRegistry()[apiName] = funcPtr;
    }
};

// 3. 供宿主开发者使用的自动注册宏
#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#define EXPORT_HOST_API(FuncName) \
    static ApiRegistrar MACRO_CONCAT(g_registrar_, __LINE__)(#FuncName, reinterpret_cast<void*>(FuncName))

#endif