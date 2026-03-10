#include "DriverManager.h"

DriverManager::DriverManager()
{
}

DriverManager::~DriverManager()
{
}

bool DriverManager::LoadAndStart(const char *pluginPath)
{
    m_handle = LOAD_LIB(pluginPath);
    if (!m_handle) return false;

    // 1. 获取三个核心入口点
    auto loadDriver = (PFN_LoadDriver)GET_FUNC(m_handle, "LoadDriver");
    m_stopFunc = (PFN_StopDriver)GET_FUNC(m_handle, "StopDriver");
    m_uninstallFunc = (PFN_UninstallDriver)GET_FUNC(m_handle, "UninstallDriver");

    if (!loadDriver || !m_stopFunc || !m_uninstallFunc) {
        CLOSE_LIB(m_handle);
        return false;
    }

    //加载驱动表
    loadDriver(&m_driverTable);

    return true;
}

void DriverManager::Unload()
{
    if (m_handle && m_stopFunc) {
        m_stopFunc();
        CLOSE_LIB(m_handle);
        m_handle = nullptr;
    }
}

void DriverManager::Uninstall()
{
    if (m_handle && m_stopFunc && m_uninstallFunc) {
        m_stopFunc();
        m_uninstallFunc();
        CLOSE_LIB(m_handle);
        m_handle = nullptr;
    }
    //下面执行删除驱动文件及收尾操作
    /* */
}
