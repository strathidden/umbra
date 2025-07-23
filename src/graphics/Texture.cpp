#include "Texture.h"
#include "../core/Logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::~Texture()
{
    if (m_id) glDeleteTextures(1, &m_id);
}

bool Texture::loadFromFile(const std::string& path)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);

    if (!data)
    {
        Logger::error("Failed to load texture: ", path);
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
    Logger::info("Loaded texture: ", path);
    return true;
}

void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}
