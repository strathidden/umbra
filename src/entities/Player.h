#pragma once
#include "Entity.h"
#include "../physics/PhysicsEngine.h"
#include "../animation/Animator.h"

class Player : public Entity
{
public:
    Player();
    void update(float deltaTime) override;
    void handleInput(float deltaTime);

private:
    PhysicsBody* m_physicsBody = nullptr;
    Animator* m_animator = nullptr;

    float m_speed = 300.0f;
    float m_jumpForce = 500.0f;
    bool m_facingRight = true;
};
