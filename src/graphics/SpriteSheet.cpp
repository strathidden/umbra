#include "SpriteSheet.h"
#include "../core/Logger.h"

bool SpriteSheet::loadFromFile(const std::string& filepath, const glm::ivec2& frameSize)
{
    if (!m_texture.loadFromFile(filepath))
    {
        return false;
    }

    m_frameSize = frameSize;
    m_gridSize = {
        m_texture.getWidth() / frameSize.x,
        m_texture.getHeight() / frameSize.y
    };
    m_frameCount = m_gridSize.x * m_gridSize.y;

    Logger::info("Loaded sprite sheet: ", filepath, " (", m_gridSize.x, "x", m_gridSize.y, " frames)");
    
    return true;
}

glm::vec2 SpriteSheet::getFrameCoords(size_t index) const
{
    if (index >= m_frameCount) return {0, 0};

    int x = index % m_gridSize.x;
    int y = index / m_gridSize.x;
    return {x * m_frameSize.x, y * m_frameSize.y};
}

Frame SpriteSheet::createFrame(size_t index, float duration) const
{
    Frame frame;
    frame.duration = duration;

    glm::vec2 topLeft = getFrameCoords(index);
    glm::vec2 bottomRight = topLeft + glm::vec2(m_frameSize);

    float texWidth = static_cast<float>(m_texture.getWidth());
    float texHeight = static_cast<float>(m_texture.getHeight());

    frame.texCoords[0] = {topLeft.x / texWidth, topLeft.y / texHeight};
    frame.texCoords[1] = {bottomRight.x / texWidth, topLeft.y / texHeight};
    frame.texCoords[2] = {bottomRight.x / texWidth, bottomRight.y / texHeight};
    frame.texCoords[3] = {topLeft.x / texWidth, bottomRight.y / texHeight};

    return frame;
}
