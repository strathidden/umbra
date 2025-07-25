#include "Button.h"
#include "../graphics/Renderer.h"
#include "../graphics/Font.h"

Button::Button(const std::string& text) : m_text(text) {}

void Button::render()
{
    Renderer::drawQuad(position, size, hovered ? m_hoverColor : m_color);

    if (!m_text.empty() && Font::getDefaultFont())
    {
        glm::vec2 textSize = Font::getDefaultFont()->getTextSize(m_text, 24.0f);
        glm::vec2 textPos = {
            position.x + (size.x - textSize.x) / 2.0f,
            position.y + (size.y - textSize.y) / 2.0f
        };

        Font::getDefaultFont()->renderText(m_text, textPos, 24.0f, {1.0f, 1.0f, 1.0f, 1.0f});
    }

    if (hovered)
    {
        Renderer::drawQuad(position - glm::vec2(2.0f, 2.0f), size + glm::vec2(4.0f, 4.0f), {1.0f, 1.0f, 1.0f, 0.8f});
    }
}

void Button::onHover()
{
    // play sound
}

void Button::onClick()
{
    if (m_onClick)
    {
        m_onClick();
    }
}
