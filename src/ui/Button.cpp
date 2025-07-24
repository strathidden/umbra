#include "Button.h"
#include "../graphics/Renderer.h"

Button::Button(const std::string& text) : m_text(text) {}

void Button::render()
{
    Renderer::drawQuad(position, size, hovered ? m_hoverColor : m_color);

    // TODO: render text
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
