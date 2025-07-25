#include "Texture.h"
#include <unordered_map>
#include <glm/glm.hpp>

struct Glyph
{
    glm::vec2 size;
    glm::vec2 bearing;
    glm::vec2 uvMin;
    glm::vec2 uvMax;
    float advance;
};

class Font
{
public:
    static Font* getDefaultFont() { return s_defaultFont; }
    static void setDefaultFont(Font* font) { s_defaultFont = font; }

    bool loadFormFile(const std::string& filepath, unsigned int fontSize = 24);

    void renderText(const std::string& text, const glm::vec2& position, float size, const glm::vec4& color);
    void renderTextWrapped(const std::string& text, glm::vec2 position, float size, float maxWidth, const glm::vec4& color);

    glm::vec2 getTextSize(const std::string& text, float fontSize);

    const Texture* getAtlas() const { return &m_atlas; }

private:
    static Font* s_defaultFont;
    Texture m_atlas;
    std::unordered_map<char, Glyph> m_glyphs;
    float m_lineHeight = 0.0f;
};
