#include "Sprite.h"

void Sprite::setTextureRect(const glm::vec2& position, const glm::vec2& size, const glm::vec2& textureSize)
{
    if (textureSize.x <= 0 || textureSize.y <= 0) return;

    float texWidth = textureSize.x;
    float texHeight = textureSize.y;

    texCoords[0] = { position.x / texWidth, position.y / texHeight };
    texCoords[1] = { (position.x + size.x) / texWidth, position.y / texHeight };
    texCoords[2] = { (position.x + size.x) / texWidth, (position.y + size.y) / texHeight };
    texCoords[3] = { position.x / texWidth, (position.y + size.y) / texHeight };
}
