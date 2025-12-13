//
// Created by edgar on 11/1/25.
//

#ifndef OPENGLPLAYGROUND_COLLIDER_H
#define OPENGLPLAYGROUND_COLLIDER_H

#include "ECS/Registry.h"
#include "Transform.h"

#include <glm/glm.hpp>
#include <vector>

namespace ECS::Components
{

struct Collider
{
    bool isColliding = false;
    std::vector<Entity> collidingEntities = {};
};

struct AABBCollider : Collider
{
    glm::vec3 min;
    glm::vec3 max;
    [[nodiscard]] AABBCollider GetWorldAABB(const Transform &transform) const;
};

struct SBBCollider : Collider
{
    glm::vec3 center;
    float radius;
};

struct OBBCollider : Collider
{
    glm::vec3 center;
    glm::quat rotation;
    glm::vec3 halfExtents;
    [[nodiscard]] OBBCollider GetWorldOBB(const Transform &transform) const;
};

using ColliderComponents = std::tuple<OBBCollider, AABBCollider, SBBCollider>;

template <typename Tuple, std::size_t... Is>
const Collider *GetColliderImpl(Registry &registry, const Entity entity, std::index_sequence<Is...>)
{
    const Collider *result = nullptr;
    (void) ((registry.HasComponent<std::tuple_element_t<Is, Tuple>>(entity) && (result = &registry.GetComponent<std::tuple_element_t<Is, Tuple>>(entity), true)) || ...);
    return result;
}

const Collider *GetCollider(Registry &registry, Entity entity);
} // namespace ECS::Components

#endif // OPENGLPLAYGROUND_COLLIDER_H
