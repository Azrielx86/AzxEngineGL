//
// Created by edgar on 11/1/25.
//

#ifndef OPENGLPLAYGROUND_COLLISION_SYSTEM_H
#define OPENGLPLAYGROUND_COLLISION_SYSTEM_H
#include "ECS/Components/Collider.h"
#include "ECS/ISystem.h"

namespace ECS::Systems
{
class CollisionSystem final : public ISystem
{

  public:
    void Update(Registry &registry, float deltaTime) override;
    static bool CheckCollision(const Components::AABBCollider &a, const Components::Transform &aTransform, const Components::AABBCollider &b, const Components::Transform &bTransform);
    static bool CheckCollision(const Components::SBBCollider &a, const Components::Transform &aTransform, const Components::SBBCollider &b, const Components::Transform &bTransform);
    static bool CheckCollision(const Components::OBBCollider &a, const Components::OBBCollider &b);
};
} // namespace ECS::Systems

#endif // OPENGLPLAYGROUND_COLLISION_SYSTEM_H
