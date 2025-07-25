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

bool PhysicsEngine::raycast(const glm::vec2& start, const glm::vec2& direction, RaycastHit& hit, uint32_t layerMask)
{
    float minDistance = std::numeric_limits<float>::max();
    bool hitFound = false;
    glm::vec2 normalizedDir = glm::normalize(direction);
    float maxDistance = glm::length(direction);
    
    for (auto body : s_bodies)
    {
        if (!(body->collider.layer & layerMask)) continue;
        if (body->isStatic) continue;
        
        // simple AABB raycast
        glm::vec2 halfSize = body->size * 0.5f;
        glm::vec2 center = body->position + halfSize;
        
        glm::vec2 tMin = (center - halfSize - start) / normalizedDir;
        glm::vec2 tMax = (center + halfSize - start) / normalizedDir;
        
        glm::vec2 t1 = glm::min(tMin, tMax);
        glm::vec2 t2 = glm::max(tMin, tMax);
        
        float tNear = glm::max(t1.x, t1.y);
        float tFar = glm::min(t2.x, t2.y);
        
        if (tNear < tFar && tNear > 0 && tNear < maxDistance && tNear < minDistance)
        {
            minDistance = tNear;
            hit.distance = tNear;
            hit.point = start + normalizedDir * tNear;
            
            if (t1.x > t1.y)
            {
                hit.normal = normalizedDir.x > 0 ? glm::vec2(-1, 0) : glm::vec2(1, 0);
            }
            else
            {
                hit.normal = normalizedDir.y > 0 ? glm::vec2(0, -1) : glm::vec2(0, 1);
            }
            
            hit.body = body;
            hitFound = true;
        }
    }
    
    return hitFound;
}
