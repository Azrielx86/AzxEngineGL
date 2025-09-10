//
// Created by tohka on 9/9/25.
//

#ifndef OPENGLPLAYGROUND_TRANSFORM_H
#define OPENGLPLAYGROUND_TRANSFORM_H

#include "../Types.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ECS::Components
{
struct Transform
{
    glm::vec3 translation{0.0f, 0.0f, 0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};

    Entity parent = 0;
};
} // namespace ECS::Components

#endif // OPENGLPLAYGROUND_TRANSFORM_H
