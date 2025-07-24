#include "TileMap.h"
#include "../core/ResourceManager.h"
#include "../graphics/Renderer.h"
#include "../core/Logger.h"
#include <fstream>
#include <sstream>

bool TileMap::loadFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        Logger::error("Failed to open tilemap: ", filepath);
        return false;
    }

    std::string line;
    int row = 0;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;
        int col = 0;

        while (std::getline(iss, token, ','))
        {
            if (token.empty()) continue;

            uint32_t id = std::stoi(token);
            if (id != 0)
            {
                Tile tile;
                tile.id = id - 1;
                tile.position = {col * m_tileSize.x, row * m_tileSize.y};
                tile.collidable = (id > 0);

                m_tiles.push_back(tile);
            }

            col++;
        }

        if (row == 0) m_mapSize.x = col;
        row++;
    }

    m_mapSize.y = row;

    m_tileset = ResourceManager::loadTexture("assets/tileset.png");
    if (!m_tileset)
    {
        Logger::error("Failed to load tileset");
        return false;
    }

    Logger::info("Loaded tilemap: ", filepath, " (", m_mapSize.x, "x", m_mapSize.y, " tiles)");
    return true;
}

void TileMap::render()
{
    for (const auto& tile : m_tiles)
    {
        int tilesPerRow = m_tileset->getWidth() / m_tileSize.x;
        int tileX = tile.id % tilesPerRow;
        int tileY = tile.id / tilesPerRow;

        glm::vec2 texTopLeft = {
            tileX * m_tileSize.x / static_cast<float>(m_tileset->getWidth()),
            tileY * m_tileSize.y / static_cast<float>(m_tileset->getHeight())
        };

        glm::vec2 texBottomRight = {
            (tileX + 1) * m_tileSize.x / static_cast<float>(m_tileset->getWidth()),
            (tileY + 1) * m_tileSize.y / static_cast<float>(m_tileset->getHeight())
        };

        Renderer::drawQuad(tile.position, glm::vec2(m_tileSize), {1.0f, 1.0f, 1.0f, 1.0f}, m_tileset, 1.0f, texTopLeft, texBottomRight);
    }
}
