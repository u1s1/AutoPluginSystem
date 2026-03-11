#include "DriverManager.h"

DriverManager::DriverManager()
{
}

DriverManager::~DriverManager()
{
    Unload();
}

bool DriverManager::LoadAndStart(const char *pluginPath)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_context != nullptr)
    {
        Unload();
    }
    
    // 创建新的上下文，如果加载失败，智能指针出作用域会自动清理
    auto newContext = std::make_shared<DriverContext>();
    newContext->handle = LOAD_LIB(pluginPath);
    if (!newContext->handle) return false;

    // 1. 获取三个核心入口点
    auto loadDriver = (PFN_LoadDriver)GET_FUNC(newContext->handle, "LoadDriver");
    newContext->stopFunc = (PFN_StopDriver)GET_FUNC(newContext->handle, "StopDriver");
    newContext->uninstallFunc = (PFN_UninstallDriver)GET_FUNC(newContext->handle, "UninstallDriver");

    if (!loadDriver || !newContext->stopFunc || !newContext->uninstallFunc) {
        //自动析构清除
        return false;
    }

    //加载驱动表
    if(!loadDriver(&newContext->table))
    {
        //自动析构清除
        return false;
    }

    //驱动小于最低版本
    if (newContext->table.version < LOW_SUPPORT_VERSION)
    {
        //自动析构清除
        return false;
    }
    m_context = std::move(newContext);

    return true;
}

void DriverManager::Unload()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_context.reset();
}

void DriverManager::Uninstall()
{
    std::shared_ptr<DriverContext> tempContext = nullptr;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        tempContext = m_context;
        m_context.reset();
    }
    if (tempContext != nullptr && tempContext->uninstallFunc != nullptr)
    {
        tempContext->uninstallFunc();
    }
    
    //下面执行删除驱动文件及收尾操作
    /* */
}

std::shared_ptr<const DriverDispatchTable> DriverManager::GetDriverTable()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_context == nullptr)
    {
        return nullptr;
    }
    return std::shared_ptr<const DriverDispatchTable>(m_context, &m_context->table);
}
