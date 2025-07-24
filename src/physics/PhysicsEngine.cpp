#include "PhysicsEngine.h"
#include "../core/Logger.h"

std::vector<PhysicsBody*> PhysicsEngine::s_bodies;
bool PhysicsEngine::s_collisionMatrix[32][32] = {false};

void PhysicsEngine::init()
{
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            s_collisionMatrix[i][j] = true;
        }
    }
    Logger::info("Physics engine initialized");
}

void PhysicsEngine::update(float deltaTime)
{
    for (auto body : s_bodies)
    {
        if (body->isStatic) continue;

        body->velocity.y += GRAVITY * deltaTime;

        glm::vec2 newPosition = body->position + body->velocity * deltaTime;
        body->position = newPosition;

        resolveCollisions(*body, deltaTime);
    }
}

void PhysicsEngine::shutdown()
{
    for (auto body : s_bodies)
    {
        delete body;
    }
    s_bodies.clear();
}

PhysicsBody* PhysicsEngine::createBody(const glm::vec2& position, const glm::vec2& size, bool isStatic)
{
    auto body = new PhysicsBody{position, {0,0}, size, Collider::createAABB(size), isStatic};
    s_bodies.push_back(body);
    return body;
}

void PhysicsEngine::removeBody(PhysicsBody* body)
{
    auto it = std::find(s_bodies.begin(), s_bodies.end(), body);
    if (it != s_bodies.end())
    {
        delete *it;
        s_bodies.erase(it);
    }
}

void PhysicsEngine::setCollisionLayers(uint32_t layer1, uint32_t layer2, bool shouldCollide)
{
    if (layer1 < 32 && layer2 < 32)
    {
        s_collisionMatrix[layer1][layer2] = shouldCollide;
        s_collisionMatrix[layer2][layer1] = shouldCollide;
    }
}

void PhysicsEngine::resolveCollisions(PhysicsBody& body, float deltaTime)
{
    body.onGround = false;

    for (auto other : s_bodies)
    {
        if (other == &body) continue;
        if (!s_collisionMatrix[body.collider.layer][other->collider.layer]) continue;

        CollisionInfo collision;
        if (body.collider.test(other->collider, body.position, other->position, collision))
        {
            if (collision.normal.y > 0.7f)
            {
                body.position.y = other->position.y - body.size.y;
                body.velocity.y = 0;
                body.onGround = true;
            }
            else if (collision.normal.y < -0.7f)
            {
                body.position.y = other->position.y + other->size.y;
                body.velocity.y = 0;
            }
            else if (collision.normal.x != 0)
            {
                body.position.x += collision.normal.x * collision.depth;
                body.velocity.x = 0;
            }
        }
    }
}
