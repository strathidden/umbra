#pragma once
#include <unordered_map>
#include <string>
#include "Animation.h"
#include "../graphics/Sprite.h"
#include "../entities/Entity.h"

class Animator : public Component
{
public:
    void addAnimation(const std::string& name, Animation&& animation);
    void play(const std::string& name, bool forceRestart = false);
    void update(float deltaTime) override;
    void applyToSprite(Sprite& sprite) const;

    const Animation* getCurrentAnimation() const;

private:
    std::unordered_map<std::string, Animation> m_animations;
    std::string m_currentAnimation;
};
