#pragma once
#include <glad/glad.h>
#include <string>

class Texture
{
public:
    Texture() = default;
    ~Texture();

    bool loadFromFile(const std::string& filepath);
    bool createFromData(unsigned char* data, int width, int height, int channels);
    void bind(unsigned int slot = 0) const;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    GLuint m_id = 0;
private:
    int m_width = 0, m_height = 0, m_channels = 0;
};
