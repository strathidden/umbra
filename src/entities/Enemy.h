#pragma once
#include "Entity.h"
#include "../physics/PhysicsEngine.h"
#include "../animation/Animator.h"

class Enemy : public Entity
{
public:
    Enemy();
    void update(float deltaTime) override;

private:
    PhysicsBody* m_physicsBody = nullptr;
    Animator* m_animator = nullptr;

    enum class State { PATROLLING, CHASING, ATTACKING };
    State m_state = State::PATROLLING;
    float m_patrolDistance = 300.0f;
    glm::vec2 m_startPosition;
    float m_detectionRange = 400.0f;
};
