#pragma once
#include "Texture.h"
#include <glm/glm.hpp>

class Sprite
{
public:
    Texture* texture = nullptr;
    glm::vec2 size = {100.0f, 100.0f};
    glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec2 texCoords[4] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };
    float rotation = 0.0f;

    Sprite() = default;
    Sprite(Texture* texture, const glm::vec2& size = {100.0f, 100.0f}) : texture(texture), size(size) {}

    void setTextureRect(const glm::vec2& position, const glm::vec2& size, const glm::vec2& textureSize);
};
