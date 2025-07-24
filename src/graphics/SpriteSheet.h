#pragma once
#include "Texture.h"
#include "../animation/Animation.h"
#include <glm/glm.hpp>

class SpriteSheet
{
public:
    bool loadFromFile(const std::string& filepath, const glm::ivec2& frameSize);
    const Texture* getTexture() const { return &m_texture; }
    glm::vec2 getFrameCoords(size_t index) const;
    Frame createFrame(size_t index, float duration) const;
    size_t getFrameCount() const { return m_frameCount; }

private:
    Texture m_texture;
    glm::ivec2 m_frameSize;
    glm::ivec2 m_gridSize;
    size_t m_frameCount = 0;
};
