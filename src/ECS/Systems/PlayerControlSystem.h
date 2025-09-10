//
// Created by tohka on 9/10/25.
//

#ifndef OPENGLPLAYGROUND_PLAYERCONTROLSYSTEM_H
#define OPENGLPLAYGROUND_PLAYERCONTROLSYSTEM_H
#include "ECS/ISystem.h"

namespace ECS::Systems
{
class PlayerControlSystem final : public ISystem
{
  public:
    void Update(Registry &registry, float deltaTime) override;
};
} // namespace ECS::Systems

#endif // OPENGLPLAYGROUND_PLAYERCONTROLSYSTEM_H
