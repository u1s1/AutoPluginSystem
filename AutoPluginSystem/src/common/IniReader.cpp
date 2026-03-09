#include "IniReader.h"

std::string IniReader::trim(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool IniReader::load(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::string currentSection = "";

    while (std::getline(file, line)) {
        line = trim(line);

        // 跳过空行和注释 (支持 ';' 和 '//')
        if (line.empty() || line[0] == ';' || (line.length() >= 2 && line[0] == '/' && line[1] == '/')) {
            continue;
        }

        // 解析 Section 标签: [section_name]
        if (line.front() == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.size() - 2);
            currentSection = trim(currentSection);
            continue;
        }

        // 解析 键值对: Key=Value
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = trim(line.substr(0, delimiterPos));
            std::string value = trim(line.substr(delimiterPos + 1));
            
            // 将键值对存入当前的 section 中
            data[currentSection][key] = value;
        }
    }
    
    file.close();
    return true;
}

std::vector<std::string> IniReader::getSections() const
{
    std::vector<std::string> sections;
    for (const auto& pair : data) {
        sections.push_back(pair.first);
    }
    return sections;
}

std::map<std::string, std::string> IniReader::getSectionData(const std::string &section) const
{
    auto it = data.find(section);
    if (it != data.end()) {
        return it->second;
    }
    return {}; // 如果没找到则返回空 map
}

std::string IniReader::getValue(const std::string &section, const std::string &key, const std::string &defaultValue) const
{
    auto secIt = data.find(section);
    if (secIt != data.end()) {
        auto keyIt = secIt->second.find(key);
        if (keyIt != secIt->second.end()) {
            return keyIt->second;
        }
    }
    return defaultValue;
}
