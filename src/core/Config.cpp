#include "Config.h"
#include "Logger.h"
#include <fstream>
#include <cctype>

std::unordered_map<std::string, std::string> Config::s_values;

static std::string trim(const std::string& str)
{
    size_t start = 0;
    size_t end = str.length() - 1;

    while (start <= end && std::isspace(static_cast<unsigned char>(str[start])))
    {
        start++;
    }
    while (end >= start && std::isspace(static_cast<unsigned char>(str[end])))
    {
        end--;
    }
    return (start <= end) ? str.substr(start, end - start + 1) : "";
}

void Config::load(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        Logger::warn("Config file not found: ", filepath);
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        size_t pos = line.find('=');
        if (pos != std::string::npos)
        {
            std::string key = trim(line.substr(0, pos));
            std::string value = trim(line.substr(pos + 1));

            if (!key.empty())
            {
                s_values[key] = value;
            }
        }
    }

    Logger::info("Loaded config: ", filepath);
}

void Config::save(const std::string& filepath)
{
    std::ofstream file(filepath);
    if (!file.is_open())
    {
        Logger::error("Failed to save config: ", filepath);
        return;
    }

    file << "# Umbra Configuration File\n";
    file << "# Generated automatically - do not edit manually\n\n";

    for (const auto& [key, value] : s_values)
    {
        file << key << "=" << value << "\n";
    }

    Logger::info("Saved config: ", filepath);
}

int Config::getInt(const std::string& key, int defaultValue)
{
    auto it = s_values.find(key);
    if (it != s_values.end())
    {
        const char* str = it->second.c_str();
        char* end;
        float value = std::strtof(str, &end);
        if (end != str && *end == '\0')
        {
            return value;
        }
        Logger::warn("Invalid integer value for key: ", key);
    }
    return defaultValue;
}

float Config::getFloat(const std::string& key, float defaultValue)
{
    auto it = s_values.find(key);
    if (it != s_values.end())
    {
        const char* str = it->second.c_str();
        char* end;
        float value = std::strtof(str, &end);
        if (end != str && *end == '\0')
        {
            return value;
        }
        Logger::warn("Invalid float value for key: ", key);
    }
    return defaultValue;
}

std::string Config::getString(const std::string& key, const std::string& defaultValue)
{
    auto it = s_values.find(key);
    if (it != s_values.end())
    {
        return it->second;
    }
    return defaultValue;
}

bool Config::getBool(const std::string& key, bool defaultValue)
{
    auto it = s_values.find(key);
    if (it != s_values.end())
    {
        const std::string& val = it->second;
        if (val == "true" || val == "1" || val == "yes")
        {
            return true;
        }
        if (val == "false" || val == "0" || val == "no")
        {
            return false;
        }
        Logger::warn("Invalid boolean value for key: ", key);
    }
    return defaultValue;
}

void Config::setInt(const std::string& key, int value)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", value);
    s_values[key] = buffer;
}

void Config::setFloat(const std::string& key, float value)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.6f", value);
    char* ptr = buffer + strlen(buffer) - 1;
    while (*ptr == '0' && ptr > buffer) ptr--;
    if (*ptr != '.') ptr++;
    *ptr = '\0';
    s_values[key] = buffer;
}

void Config::setString(const std::string& key, const std::string& value)
{
    s_values[key] = value;
}

void Config::setBool(const std::string& key, bool value)
{
    s_values[key] = value ? "true" : "false";
}
