#include "Collider.h"
#include <cmath>

bool Collider::test(const Collider& other, const glm::vec2& posA, const glm::vec2& posB, CollisionInfo& info) const
{
    if (type == AABB && other.type == AABB)
    {
        glm::vec2 halfSizeA = size * 0.5f;
        glm::vec2 halfSizeB = other.size * 0.5f;

        glm::vec2 centerA = posA + halfSizeA;
        glm::vec2 centerB = posB + halfSizeB;

        glm::vec2 difference = centerB - centerA;
        glm::vec2 overlap = halfSizeA + halfSizeB - glm::abs(difference);

        if (overlap.x < 0 || overlap.y < 0) return false;

        if (overlap.x < overlap.y)
        {
            info.depth = overlap.x;
            info.normal = glm::vec2(difference.x < 0 ? 1.0f : -1.0f, 0.0f);
        }
        else
        {
            info.depth = overlap.y;
            info.normal = glm::vec2(0.0f, difference.y < 0 ? 1.0f : -1.0f);
        }

        return true;
    }

    // Other collision types...

    return false;
}
