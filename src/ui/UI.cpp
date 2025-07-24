#include "UI.h"
#include "../core/InputManager.h"

std::vector<UIElement*> UIManager::s_elements;

void UIManager::init() {}

void UIManager::shutdown()
{
    s_elements.clear();
}

void UIManager::addElement(UIElement* element)
{
    s_elements.push_back(element);
}

void UIManager::removeElement(UIElement* element)
{
    auto it = std::find(s_elements.begin(), s_elements.end(), element);
    if (it != s_elements.end())
    {
        s_elements.erase(it);
    }
}

void UIManager::update(float deltaTime)
{
    glm::vec2 mousePos = InputManager::getMousePosition();

    for (auto element : s_elements)
    {
        if (!element->visible) continue;

        bool wasHovered = element->hovered;
        element->hovered = mousePos.x >= element->position.x && mousePos.y >= element->position.y && mousePos.x <= element->position.x + element->size.x && mousePos.y <= element->position.y + element->size.y;

        if (element->hovered && !wasHovered)
        {
            element->onHover();
        }

        if (element->hovered && InputManager::isMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
        {
            element->onClick();
        }
        
        element->update(deltaTime);
    }
}

void UIManager::render()
{
    for (auto element : s_elements)
    {
        if (element->visible)
        {
            element->render();
        }
    }
}
