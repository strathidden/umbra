#pragma once
#include <unordered_map>
#include <string>

class Config
{
public:
    static void load(const std::string& filepath);
    static void save(const std::string& filepath);

    static int getInt(const std::string& key, int defaultValue = 0);
    static float getFloat(const std::string& key, float defaultValue = 0.0f);
    static std::string getString(const std::string& key, const std::string& defaultValue = "");
    static bool getBool(const std::string& key, bool defaultValue = false);

    static void setInt(const std::string& key, int value);
    static void setFloat(const std::string& key, float value);
    static void setString(const std::string& key, const std::string& value);
    static void setBool(const std::string& key, bool value);

private:
    static std::unordered_map<std::string, std::string> s_values;
};
