#pragma once
#include "Collider.h"
#include "../entities/Entity.h"
#include <vector>
#include <glm/glm.hpp>

struct PhysicsBody : public Component
{
    // schizo FIX
    PhysicsBody(const glm::vec2& position, const glm::vec2& size) : position(position), size(size), collider(Collider::createAABB(size)) {}

    PhysicsBody(const glm::vec2& position, const glm::vec2& velocity, const glm::vec2& size, Collider collider, bool isStatic = false, bool onGround = false) : position(position), velocity(velocity), size(size), collider(std::move(collider)), isStatic(isStatic) {}
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 size;
    Collider collider;
    bool isStatic;
    bool onGround;
};

struct RaycastHit
{
    glm::vec2 point;
    glm::vec2 normal;
    float distance;
    PhysicsBody* body = nullptr;
};

class PhysicsEngine
{
public:
    static constexpr float GRAVITY = 980.0f;

    static void init();
    static void update(float deltaTime);
    static void shutdown();

    static PhysicsBody* createBody(const glm::vec2& position, const glm::vec2& size, bool isStatic = false);
    static void removeBody(PhysicsBody* body);

    static void setCollisionLayers(uint32_t layer1, uint32_t layer2, bool shouldCollide);

    static bool raycast(const glm::vec2& start, const glm::vec2& direction, RaycastHit& hit, uint32_t layerMask = 0xFFFFFFFF);

private:
    static void resolveCollisions(PhysicsBody& body, float deltaTime);
    
    static std::vector<PhysicsBody*> s_bodies;
    static bool s_collisionMatrix[32][32];
};
