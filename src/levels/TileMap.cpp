#include "TileMap.h"
#include "../core/ResourceManager.h"
#include "../graphics/Renderer.h"
#include "../core/Logger.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void TileMap::updateTileWorldPosition(Tile& tile)
{
    tile.worldPosition = {
        tile.gridPosition.x * m_tileSize.x,
        tile.gridPosition.y * m_tileSize.y
    };
}

bool TileMap::loadFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        Logger::error("Failed to open tilemap: ", filepath);
        return false;
    }

    try
    {
        json data = json::parse(file);

        m_mapSize.x = data["width"];
        m_mapSize.y = data["height"];
        m_tileSize.x = data["tile_size"];
        m_tileSize.y = m_tileSize.x; // square tiles

        std::string tilesetPath = data["tileset"];
        m_tileset = ResourceManager::loadTexture(tilesetPath);

        m_tiles.clear();

        for (const auto& tileData : data["tiles"])
        {
            Tile tile;
            tile.gridPosition.x = tileData["x"];
            tile.gridPosition.y = tileData["y"];
            tile.id = tileData["id"];
            tile.collidable = tileData["collidable"];
            updateTileWorldPosition(tile);
            m_tiles.push_back(tile);
        }

        Logger::info("Loaded tilemap: ", filepath, " (", m_mapSize.x, "x", m_mapSize.y, " tiles)");

        return true;
    }
    catch (const json::exception& e)
    {
        Logger::error("JSON parse error: ", e.what());
        return false;
    }
}

void TileMap::createEmpty(int width, int height, int tileSize)
{
    m_tiles.clear();
    m_mapSize = {width, height};
    m_tileSize = {tileSize, tileSize};
}

void TileMap::setTile(int gridX, int gridY, uint32_t tileId, bool collidable)
{
    if (gridX < 0 || gridX >= m_mapSize.x || gridY < 0 || gridY >= m_mapSize.y)
    {
        return;
    }

    for (auto& tile : m_tiles)
    {
        if (tile.gridPosition.x == gridX && tile.gridPosition.y == gridY)
        {
            tile.id = tileId;
            tile.collidable = collidable;
            return;
        }
    }

    Tile newTile;
    newTile.gridPosition = {gridX, gridY};
    newTile.id = tileId;
    newTile.collidable = collidable;
    updateTileWorldPosition(newTile);
    m_tiles.push_back(newTile);
}

void TileMap::clearTile(int gridX, int gridY)
{
    m_tiles.erase(std::remove_if(m_tiles.begin(), m_tiles.end(), [gridX, gridY](const Tile& tile) {
            return tile.gridPosition.x == gridX && tile.gridPosition.y == gridY;
        }),
        m_tiles.end()
    );
}

Tile* TileMap::getTileAt(int gridX, int gridY)
{
    for (auto& tile : m_tiles)
    {
        if (tile.gridPosition.x == gridX && tile.gridPosition.y == gridY)
        {
            return &tile;
        }
    }
    return nullptr;
}

void TileMap::fillArea(int startX, int startY, int endX, int endY, uint32_t tileId, bool collidable)
{
    int minX = std::min(startX, endX);
    int maxX = std::max(startX, endX);
    int minY = std::min(startY, endY);
    int maxY = std::max(startY, endY);

    minX = std::max(0, minX);
    maxX = std::min(m_mapSize.x - 1, maxX);
    minY = std::max(0, minY);
    maxY = std::min(m_mapSize.y - 1, maxY);

    for (int y = minY; y <= maxY; y++)
    {
        for (int x = minX; x <= maxX; x++)
        {
            setTile(x, y, tileId, collidable);
        }
    }
}

void TileMap::render()
{
    for (const auto& tile : m_tiles)
    {
        if (tile.id == 0) continue;

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

        Renderer::drawQuad(tile.worldPosition, glm::vec2(m_tileSize), {1.0f, 1.0f, 1.0f, 1.0f}, m_tileset, 1.0f, texTopLeft, texBottomRight);
    }
}
