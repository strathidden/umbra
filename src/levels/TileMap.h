#pragma once
#include "../graphics/Texture.h"
#include <vector>
#include <glm/glm.hpp>

struct Tile
{
    uint32_t id;
    glm::ivec2 gridPosition;
    glm::vec2 worldPosition;
    bool collidable;
};

class TileMap
{
public:
    bool loadFromFile(const std::string& filepath);
    void createEmpty(int width, int height, int tileSize);
    void render();
    const std::vector<Tile>& getTiles() const { return m_tiles; }
    const glm::ivec2& getTileSize() const { return m_tileSize; }
    const glm::ivec2& getMapSize() const { return m_mapSize; }
    Texture* getTileset() { return m_tileset; }

    void setTile(int gridX, int gridY, uint32_t tileId, bool collidable = true);
    void clearTile(int gridX, int gridY);
    void fillArea(int startX, int startY, int endX, int endY, uint32_t tileId, bool collidable = true);
    Tile* getTileAt(int gridX, int gridY);

private:
    void updateTileWorldPosition(Tile& tile);

    std::vector<Tile> m_tiles;
    glm::ivec2 m_tileSize = {64, 64};
    glm::ivec2 m_mapSize = {0, 0};
    Texture* m_tileset;
};
