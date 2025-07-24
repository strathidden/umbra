#pragma once
#include "../graphics/Texture.h"
#include <vector>
#include <glm/glm.hpp>

struct Tile
{
    uint32_t id;
    glm::vec2 position;
    bool collidable;
};

class TileMap
{
public:
    bool loadFromFile(const std::string& filepath);
    void render();
    const std::vector<Tile>& getTiles() const { return m_tiles; }
    const glm::ivec2& getTileSize() const { return m_tileSize; }
    const glm::ivec2& getMapSize() const { return m_mapSize; }

private:
    std::vector<Tile> m_tiles;
    glm::ivec2 m_tileSize = {64, 64};
    glm::ivec2 m_mapSize = {0, 0};
    Texture* m_tileset;
};
