//
// Created by tohka on 9/10/25.
//

#ifndef OPENGLPLAYGROUND_RENDERSYSTEM_H
#define OPENGLPLAYGROUND_RENDERSYSTEM_H

#include "ECS/ISystem.h"
#include <glm/glm.hpp>

namespace ECS::Systems
{
class RenderSystem final : public ISystem
{
    static glm::mat4 GetWorldTransform(Registry &registry, Entity entity);

  public:
    void Update(Registry &registry, float deltaTime) override;
    static void RenderEntity(Registry &registry, Entity entity);
};
} // namespace ECS::Systems

#endif // OPENGLPLAYGROUND_RENDERSYSTEM_H
