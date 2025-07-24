#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class SaveManager
{
public:
    struct SaveData
    {
        std::string level;
        glm::vec2 playerPosition;
        std::unordered_map<std::string, bool> switches;
        std::unordered_map<std::string, int> items;
    };

    static bool save(const std::string& slot, const SaveData& data);
    static bool load(const std::string& slot, SaveData& data);
    static std::vector<std::string> getSaveSlots();

private:
    static std::string getSavePath(const std::string& slot);
};
