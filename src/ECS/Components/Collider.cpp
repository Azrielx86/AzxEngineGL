#include "Collider.h"

ECS::Components::AABBCollider ECS::Components::AABBCollider::GetWorldAABB(const Transform &transform) const
{
    const glm::vec3 worldMin = transform.translation + min * transform.scale;
    const glm::vec3 worldMax = transform.translation + max * transform.scale;
    return {worldMin, worldMax};
}
