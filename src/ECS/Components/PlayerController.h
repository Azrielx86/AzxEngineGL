//
// Created by tohka on 9/10/25.
//

#ifndef OPENGLPLAYGROUND_PLAYERCONTROLLER_H
#define OPENGLPLAYGROUND_PLAYERCONTROLLER_H

namespace ECS::Components
{
struct PlayerController
{
    float moveSpeed = 2.0f;
    float turnSpeed = 120.0f;
    bool enabled = false;
};
} // namespace ECS::Components

#endif // OPENGLPLAYGROUND_PLAYERCONTROLLER_H
