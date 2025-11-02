//
// Created by edgar on 11/1/25.
//

#ifndef OPENGLPLAYGROUND_COLLIDER_H
#define OPENGLPLAYGROUND_COLLIDER_H

#include "Transform.h"
#include <glm/glm.hpp>
#include <vector>

namespace ECS::Components
{

struct AABBCollider
{
    glm::vec3 min;
    glm::vec3 max;
    [[nodiscard]] AABBCollider GetWorldAABB(const Transform &transform) const;
};

struct SBBCollider
{
    glm::vec3 center;
    float radius;
};

struct OBBCollider
{
    glm::vec3 center;
    glm::vec3 halfExtents;
};
} // namespace ECS::Components

#endif // OPENGLPLAYGROUND_COLLIDER_H
