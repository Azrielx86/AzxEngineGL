#include "Collider.h"

ECS::Components::AABBCollider ECS::Components::AABBCollider::GetWorldAABB(const Transform &transform) const
{
    const glm::vec3 worldMin = transform.translation + min * transform.scale;
    const glm::vec3 worldMax = transform.translation + max * transform.scale;
    return {.min = worldMin, .max = worldMax};
}

ECS::Components::OBBCollider ECS::Components::OBBCollider::GetWorldOBB(const Transform &transform) const
{
    const glm::vec3 c = transform.translation + transform.rotation * (center * transform.scale);
    const glm::quat r = transform.rotation * rotation;
    const glm::vec3 h = halfExtents * transform.scale;
    return {.center = c, .rotation = r, .halfExtents = h};
}

const ECS::Components::Collider *ECS::Components::GetCollider(Registry &registry, const Entity entity)
{
    return GetColliderImpl<ColliderComponents>(registry, entity, std::make_index_sequence<std::tuple_size_v<ColliderComponents>>{});
}
