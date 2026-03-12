#ifndef INIOPERATOR_H
#define INIOPERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <mutex>
#include "IConfigParser.h"

class IniOperator :public IConfigParser
{
private:
    std::string m_fileName;
    bool m_isDirty;
    std::mutex m_mutex;
    // 数据结构: map<Section名, map<键, 值>>
    std::map<std::string, std::map<std::string, std::string>> m_data;

    // 辅助函数：去除字符串首尾的空格和换行符
    static std::string trim(const std::string &str);

public:
    IniOperator(const std::string &filename = "");
    ~IniOperator();
    // 加载并解析 INI 文件
    bool load(const std::string &filename) override;

    bool save() override;

    bool replaceAllData(const std::map<std::string, std::map<std::string, std::string>>& newData) override;

    // 获取动态读取到的所有 Section 名称
    std::vector<std::string> getAllSections() override;

    // 获取特定 Section 下的所有键值对
    std::map<std::string, std::string> getSectionData(const std::string &section) override;

    bool setSectionData(const std::string& section, 
        const std::map<std::string, std::string>& sectionData) override;

    // 获取具体的某一个值 (带默认值保护)
    std::string getValue(const std::string &section, const std::string &key, const std::string &defaultValue = "") override;

    bool setValue(const std::string &section, const std::string &key, const std::string &value) override;
};

#endif