#include "Texture.h"
#include "../core/Logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::~Texture()
{
    if (m_id) glDeleteTextures(1, &m_id);
}

bool Texture::loadFromFile(const std::string& filepath)
{
    unsigned char* data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);

    if (!data)
    {
        Logger::error("Failed to load texture: ", filepath);
        return false;
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    Logger::info("Loaded texture: ", filepath);
    return true;
}

bool Texture::createFromData(unsigned char* data, int width, int height, int channels)
{
    if (m_id)
    {
        glDeleteTextures(1, &m_id);
    }

    m_width = width;
    m_height = height;
    m_channels = channels;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    GLenum format;
    switch (channels)
    {
        case 1: format = GL_RED; break;
        case 2: format = GL_RGB; break;
        case 3: format = GL_RGBA; break;
        default:
            Logger::error("Unsupported number of channels: ", channels);
            return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    Logger::info("Created texture from data (", width, "x", height, ", channels: ", channels, ")");
    return true;
}

void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}
