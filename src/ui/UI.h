#pragma once
#include <vector>
#include <glm/glm.hpp>

class UIElement
{
public:
    virtual ~UIElement() = default;
    virtual void update(float deltaTime) {}
    virtual void render() {}
    virtual void onHover() {}
    virtual void onClick() {}

    glm::vec2 position = {0.0f, 0.0f};
    glm::vec2 size = {100.0f, 50.0f};
    bool visible = true;
    bool hovered = false;
};

class UIManager
{
public:
    static void init();
    static void shutdown();

    static void addElement(UIElement* element);
    static void removeElement(UIElement* element);

    static void update(float deltaTime);
    static void render();

private:
    static std::vector<UIElement*> s_elements;
};
