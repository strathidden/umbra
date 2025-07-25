#include "Animator.h"

void Animator::addAnimation(const std::string& name, Animation&& animation)
{
    m_animations.emplace(name, std::move(animation));
}

void Animator::play(const std::string& name, bool forceRestart)
{
    auto it = m_animations.find(name);
    if (it == m_animations.end()) return;

    if (forceRestart || m_currentAnimation != name)
    {
        m_currentAnimation = name;
        it->second.reset();
    }
}

void Animator::update(float deltaTime)
{
    auto it = m_animations.find(m_currentAnimation);
    if (it != m_animations.end())
    {
        it->second.update(deltaTime);
    }
}

void Animator::applyToSprite(Sprite& sprite) const
{
    auto it = m_animations.find(m_currentAnimation);
    if (it != m_animations.end())
    {
        const auto& frame = it->second.getCurrentFrame();
        for (int i = 0; i < 4; i++)
        {
            sprite.texCoords[i] = frame.texCoords[i];
        }
    }
}

const Animation* Animator::getCurrentAnimation() const
{
    auto it = m_animations.find(m_currentAnimation);
    if (it != m_animations.end())
    {
        return &it->second;
    }
    return nullptr;
}
