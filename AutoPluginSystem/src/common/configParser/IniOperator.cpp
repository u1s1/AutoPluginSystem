#include "IniOperator.h"

std::string IniOperator::trim(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

IniOperator::IniOperator(const std::string &filename) : 
    m_fileName(filename), 
    m_isDirty(false)
{
}

IniOperator::~IniOperator()
{
    if (m_isDirty)
    {
        save();
    }
}

bool IniOperator::load(const std::string &filename)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filename << std::endl;
        return false;
    }
    m_fileName = filename;
    m_data.clear();
    m_isDirty = false;

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
            m_data[currentSection][key] = value;
        }
    }
    
    file.close();
    return true;
}

bool IniOperator::save()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ofstream file(m_fileName);
    if (!file.is_open()) {
        std::cout << "Failed to open file for writing: " << m_fileName << std::endl;
        return false;
    }

    // 遍历所有 Section
    for (const auto& sectionPair : m_data) {
        // 写入 Section 头
        file << "[" << sectionPair.first << "]\n";
        
        // 遍历并写入该 Section 下的所有键值对
        for (const auto& keyValuePair : sectionPair.second) {
            file << keyValuePair.first << "=" << keyValuePair.second << "\n";
        }
        
        // 每个 Section 之间加一个空行，保持排版美观
        file << "\n"; 
    }

    file.close();
    m_isDirty = false;
    return true;
}

bool IniOperator::replaceAllData(const std::map<std::string, std::map<std::string, std::string>> &newData)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data = newData;
    m_isDirty = true;
    return true;
}

std::vector<std::string> IniOperator::getAllSections()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<std::string> sections;
    for (const auto& pair : m_data) {
        sections.push_back(pair.first);
    }
    return sections;
}

std::map<std::string, std::string> IniOperator::getSectionData(const std::string &section)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_data.find(section);
    if (it != m_data.end()) {
        return it->second;
    }
    return {}; // 如果没找到则返回空 map
}

bool IniOperator::setSectionData(const std::string& section, const std::map<std::string, std::string>& sectionData)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for(const auto& it : sectionData)
    {
        m_data[section][it.first] = it.second;
    }
    m_isDirty = true;
    return true;
}

std::string IniOperator::getValue(const std::string &section, const std::string &key, const std::string &defaultValue)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto secIt = m_data.find(section);
    if (secIt != m_data.end()) {
        auto keyIt = secIt->second.find(key);
        if (keyIt != secIt->second.end()) {
            return keyIt->second;
        }
    }
    return defaultValue;
}

bool IniOperator::setValue(const std::string &section, const std::string &key, const std::string &value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data[section][key] = value;
    m_isDirty = true;
    return true;
}
