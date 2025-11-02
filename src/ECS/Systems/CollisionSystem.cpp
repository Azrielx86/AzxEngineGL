//
// Created by edgar on 11/1/25.
//

#include "CollisionSystem.h"

#include <format>
#include <glm/gtx/quaternion.hpp>

void ECS::Systems::CollisionSystem::Update(Registry &registry, [[maybe_unused]] float deltaTime)
{
    const auto aabbEntities = registry.View<Components::Transform, Components::AABBCollider>();
    const auto sbbEntities = registry.View<Components::Transform, Components::SBBCollider>();

    /*
     * AABB vs AABB
     */
    for (size_t i = 0; i < aabbEntities.size(); ++i)
    {
        for (size_t j = 0; j < aabbEntities.size(); ++j)
        {
            if (i == j) continue;
            Entity entityA = aabbEntities[i];
            Entity entityB = aabbEntities[j];

            auto &colliderA = registry.GetComponent<Components::AABBCollider>(entityA);
            auto &transformA = registry.GetComponent<Components::Transform>(entityA);

            auto &colliderB = registry.GetComponent<Components::AABBCollider>(entityB);
            auto &transformB = registry.GetComponent<Components::Transform>(entityB);

            const bool collision = CheckCollision(colliderA, transformA, colliderB, transformB);

#if ENABLE_LOG
            if (collision)
                std::cout << std::format("AABB vs AABB Collision by entities {} and {}\n", entityA, entityB);
#endif
        }
    }

    /*
     * SBB vs SBB
     */
    for (size_t i = 0; i < sbbEntities.size(); ++i)
    {
        for (size_t j = 0; j < sbbEntities.size(); ++j)
        {
            if (i == j) continue;
            Entity entityA = sbbEntities[i];
            Entity entityB = sbbEntities[j];

            auto &colliderA = registry.GetComponent<Components::SBBCollider>(entityA);
            auto &transformA = registry.GetComponent<Components::Transform>(entityA);

            auto &colliderB = registry.GetComponent<Components::SBBCollider>(entityB);
            auto &transformB = registry.GetComponent<Components::Transform>(entityB);

            const bool collision = CheckCollision(colliderA, transformA, colliderB, transformB);

#if ENABLE_LOG
            if (collision)
                std::cout << std::format("SBB vs SBB Collision by entities {} and {}\n", entityA, entityB);
#endif
        }
    }
}

bool ECS::Systems::CollisionSystem::CheckCollision(const Components::AABBCollider &a, const Components::Transform &aTransform, const Components::AABBCollider &b, const Components::Transform &bTransform)
{
    const auto worldA = a.GetWorldAABB(aTransform);
    const auto worldB = b.GetWorldAABB(bTransform);
    const bool overlapX = worldA.min.x <= worldB.max.x && worldA.max.x >= worldB.min.x;
    const bool overlapY = worldA.min.y <= worldB.max.y && worldA.max.y >= worldB.min.y;
    const bool overlapZ = worldA.min.z <= worldB.max.z && worldA.max.z >= worldB.min.z;
    return overlapX && overlapY && overlapZ;
}

bool ECS::Systems::CollisionSystem::CheckCollision(const Components::SBBCollider &a, const Components::Transform &aTransform, const Components::SBBCollider &b, const Components::Transform &bTransform)
{
    const glm::vec3 worldCenterA = aTransform.translation + a.center;
    const glm::vec3 worldCenterB = bTransform.translation + b.center;
    const float radiusA = a.radius * glm::max(aTransform.scale.x, glm::max(aTransform.scale.y, aTransform.scale.z));
    const float radiusB = b.radius * glm::max(bTransform.scale.x, glm::max(bTransform.scale.y, bTransform.scale.z));

    const float distanceSq = glm::length2(worldCenterA - worldCenterB);
    const float radSum = radiusA + radiusB;

    return distanceSq <= radSum * radSum;
}