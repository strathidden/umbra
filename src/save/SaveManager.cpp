#include "SaveManager.h"
#include "../core/Logger.h"
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool SaveManager::save(const std::string& slot, const SaveData& data)
{
    json j;
    j["level"] = data.level;
    j["player"]["position"]["x"] = data.playerPosition.x;
    j["player"]["position"]["y"] = data.playerPosition.y;
    
    for (const auto& [id, state] : data.switches)
    {
        j["switches"][id] = state;
    }

    for (const auto& [id, count] : data.items)
    {
        j["items"][id] = count;
    }

    std::ofstream file(getSavePath(slot));
    if (!file.is_open())
    {
        Logger::error("Falied to save game: ", slot);
        return false;
    }

    file << j.dump(4);
    return true;
}

bool SaveManager::load(const std::string& slot, SaveData& data)
{
    std::ifstream file(getSavePath(slot));
    if (!file.is_open())
    {
        Logger::warn("Save file not found: ", slot);
        return false;
    }

    try
    {
        json j = json::parse(file);
        data.level = j["level"];
        data.playerPosition.x = j["player"]["position"]["x"];
        data.playerPosition.y = j["player"]["position"]["y"];

        if (j.contains("switches"))
        {
            for (const auto& [id, state] : j["switches"].items())
            {
                data.switches[id] = state;
            }
        }

        if (j.contains("items"))
        {
            for (const auto& [id, count] : j["items"].items())
            {
                data.items[id] = count;
            }
        }

        return true;
    }
    catch (const json::exception& e)
    {
        Logger::error("Save file corrupted: ", e.what());
        return false;
    }
}

std::vector<std::string> SaveManager::getSaveSlots()
{
    std::vector<std::string> slots;
    for (const auto& entry : std::filesystem::directory_iterator("saves"))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".sav")
        {
            slots.push_back(entry.path().stem().string());
        }
    }
    return slots;
}

std::string SaveManager::getSavePath(const std::string& slot)
{
    return "saves/" + slot + ".sav";
}
