#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <typeindex>
#include <unordered_map>

class Component
{
public:
    virtual ~Component() = default;
    virtual void update(float deltaTime) {}
    virtual void render() {}
};

class Entity
{
public:
    template<typename T, typename... Args>
    T* addComponent(Args&&... args)
    {
        std::unique_ptr<Component> component = std::make_unique<T>(std::forward<Args>(args)...);
        T* rawPtr = static_cast<T*>(component.get());
        m_components[typeid(T)] = std::move(component);
        return rawPtr;
    }

    template<typename T>
    T* getComponent()
    {
        auto it = m_components.find(typeid(T));
        if (it != m_components.end())
        {
            return dynamic_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    virtual void update(float deltaTime)
    {
        for (auto& [type, component] : m_components)
        {
            component->update(deltaTime);
        }
    }

    virtual void render()
    {
        for (auto& [type, component] : m_components)
        {
            component->render();
        }
    }

    glm::vec2 position = {0.0f, 0.0f};
    glm::vec2 scale = {1.0f, 1.0f};
    float rotation = 0.0f;
    bool active = true;

private:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;
};
