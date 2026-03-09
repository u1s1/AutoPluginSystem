#ifndef INIREADER_H
#define INIREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

class IniReader {
private:
    // 数据结构: map<Section名, map<键, 值>>
    std::map<std::string, std::map<std::string, std::string>> data;

    // 辅助函数：去除字符串首尾的空格和换行符
    static std::string trim(const std::string &str);

public:
    // 加载并解析 INI 文件
    bool load(const std::string &filename);

    // 获取动态读取到的所有 Section 名称
    std::vector<std::string> getSections() const;

    // 获取特定 Section 下的所有键值对
    std::map<std::string, std::string> getSectionData(const std::string &section) const;

    // 获取具体的某一个值 (带默认值保护)
    std::string getValue(const std::string &section, const std::string &key, const std::string &defaultValue = "") const;
};

#endif