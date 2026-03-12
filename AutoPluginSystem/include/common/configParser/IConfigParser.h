#ifndef I_CONFIG_PARSER_H
#define I_CONFIG_PARSER_H

#include <string>
#include <map>
#include <vector>

class IConfigParser
{
public:
    virtual ~IConfigParser() = default;
    virtual bool load(const std::string &filename) = 0;
    virtual bool save() = 0;
    //替换所有旧数据
    virtual bool replaceAllData(const std::map<std::string, std::map<std::string, std::string>> &newData) = 0;
    // 获取动态读取到的所有 Section 名称
    virtual std::vector<std::string> getAllSections() = 0;
    // 获取特定 Section 下的所有键值对
    virtual std::map<std::string, std::string> getSectionData(const std::string &section) = 0;
    // 设定特定 Section 下的键值对
    virtual bool setSectionData(const std::string& section, 
        const std::map<std::string, std::string>& sectionData) = 0;
    // 获取具体的某一个值 (带默认值保护)
    virtual std::string getValue(const std::string &section, const std::string &key, 
        const std::string &defaultValue = "") = 0;
    virtual bool setValue(const std::string &section, const std::string &key, const std::string &value) = 0;
};

#endif // I_CONFIG_PARSER_H