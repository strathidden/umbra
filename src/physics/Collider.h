#pragma once
#include <glm/glm.hpp>

struct CollisionInfo
{
    glm::vec2 normal;
    float depth;
};

class Collider
{
public:
    enum Type { NONE, AABB, CIRCLE };

    Type type = NONE;
    uint32_t layer = 0;

    static Collider createAABB(const glm::vec2& size)
    {
        Collider collider;
        collider.type = AABB;
        collider.size = size;
        return collider;
    }

    static Collider createCircle(float radius)
    {
        Collider collider;
        collider.type = CIRCLE;
        collider.radius = radius;
        return collider;
    }

    bool test(const Collider& other, const glm::vec2& posA, const glm::vec2& posB, CollisionInfo& info) const;

    union
    {
        glm::vec2 size;
        float radius;
    };
};
