#pragma once
#include "UI.h"
#include <functional>
#include <string>

class Button : public UIElement
{
public:
    Button(const std::string& text = "");

    void render() override;
    void onHover() override;
    void onClick() override;

    void setOnClick(std::function<void()> callback) { m_onClick = callback; }

private:
    std::string m_text;
    glm::vec4 m_color = {0.2f, 0.3f, 0.8f, 1.0f};
    glm::vec4 m_hoverColor = {0.3f, 0.4f, 0.9f, 1.0f};
    std::function<void()> m_onClick;
};
