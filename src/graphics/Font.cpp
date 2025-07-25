#include "Font.h"
#include "../core/Logger.h"
#include "../graphics/Renderer.h"
#include <ft2build.h>
#include FT_FREETYPE_H

Font* Font::s_defaultFont = nullptr;

bool Font::loadFormFile(const std::string& filepath, unsigned int fontSize)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        Logger::error("FREETYPE: Could not init FreeType Library");
        return false;
    }

    FT_Face face;
    if (FT_New_Face(ft, filepath.c_str(), 0, &face))
    {
        Logger::error("FREETYPE: Failed to load font: ", filepath);
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned int atlasWidth = 0;
    unsigned int atlasHeight = 0;
    m_lineHeight = 0;

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            Logger::warn("FREETYPE: Failed to load Glyph: ", c);
            continue;
        }

        atlasWidth += face->glyph->bitmap.width;
        atlasHeight = std::max(atlasHeight, face->glyph->bitmap.rows);
        m_lineHeight = std::max(m_lineHeight, static_cast<float>(face->glyph->bitmap.rows));
    }

    unsigned char* atlasData = new unsigned char[atlasWidth * atlasHeight * 4]();
    unsigned int xOffset = 0;

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            continue;
        }

        Glyph glyph;
        glyph.size = {face->glyph->bitmap.width, face->glyph->bitmap.rows};
        glyph.bearing = {face->glyph->bitmap_left, face->glyph->bitmap_top};
        glyph.advance = face->glyph->advance.x >> 6; // get value in pixels

        for (unsigned int y = 0; y < glyph.size.y; y++)
        {
            for (unsigned int x = 0; x < glyph.size.x; x++)
            {
                unsigned int index = (y * atlasWidth + xOffset + x) * 4;
                unsigned char value = face->glyph->bitmap.buffer[y * static_cast<unsigned int>(glyph.size.x) + x];
                atlasData[index + 0] = 255;
                atlasData[index + 1] = 255;
                atlasData[index + 2] = 255;
                atlasData[index + 3] = value;
            }
        }

        glyph.uvMin = {static_cast<float>(xOffset) / atlasWidth, 0.0f};
        glyph.uvMax = {static_cast<float>(xOffset + glyph.size.x) / atlasWidth, static_cast<float>(glyph.size.y) / atlasHeight};
        
        m_glyphs[c] = glyph;
        xOffset += glyph.size.x;
    }

    m_atlas.createFromData(atlasData, atlasWidth, atlasHeight, 4);
    delete[] atlasData;

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    Logger::info("Loaded font: ", filepath, " (", fontSize, "px)");
    return true;
}

void Font::renderText(const std::string& text, const glm::vec2& position, float size, const glm::vec4& color)
{
    float scale = size / m_lineHeight;
    glm::vec2 cursor = position;

    for (const char c : text)
    {
        const Glyph& glyph = m_glyphs[c];

        glm::vec2 glyphPos = cursor + glm::vec2(glyph.bearing.x, -glyph.bearing.y) * scale;
        glm::vec2 glyphSize = glyph.size * scale;

        Renderer::drawQuad(glyphPos, glyphSize, color, &m_atlas, 1.0f, glyph.uvMin, glyph.uvMax);

        cursor.x += glyph.advance * scale;
    }
}

void Font::renderTextWrapped(const std::string& text, glm::vec2 position, float size, float maxWidth, const glm::vec4& color)
{
    float scale = size / m_lineHeight;
    glm::vec2 cursor = position;
    float lineStartX = position.x;
    float spaceAdvance = m_glyphs[' '].advance * scale;

    std::istringstream iss(text);
    std::string word;

    while (iss >> word)
    {
        float wordWidth = 0.0f;
        for (char c : word)
        {
            wordWidth += m_glyphs[c].advance * scale;
        }

        if (cursor.x + wordWidth > position.x + maxWidth)
        {
            cursor.x = lineStartX;
            cursor.y += m_lineHeight * scale * 1.5f;
        }

        for (char c : word)
        {
            const Glyph& glyph = m_glyphs[c];

            glm::vec2 glyphPos = cursor + glm::vec2(glyph.bearing.x, -glyph.bearing.y) * scale;
            glm::vec2 glyphSize = glyph.size * scale;

            Renderer::drawQuad(glyphPos, glyphSize, color, &m_atlas, 1.0f, glyph.uvMin, glyph.uvMax);

            cursor.x += glyph.advance * scale;
        }

        cursor.x += spaceAdvance;
    }
}

glm::vec2 Font::getTextSize(const std::string& text, float fontSize)
{
    float scale = fontSize / m_lineHeight;
    float width = 0.0f;
    float height = m_lineHeight * scale;

    for (const char c : text)
    {
        width += m_glyphs[c].advance * scale;
    }

    return {width, height};
}
