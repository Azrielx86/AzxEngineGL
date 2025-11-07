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
    const auto obbEntities = registry.View<Components::Transform, Components::OBBCollider>();

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

    /*
     * OBB vs OBB
     */
    for (Entity obbEntityA : obbEntities)
    {
        auto &colliderA = registry.GetComponent<Components::OBBCollider>(obbEntityA);
        auto &transformA = registry.GetComponent<Components::Transform>(obbEntityA);
        auto worldOBBA = colliderA.GetWorldOBB(transformA);

        for (Entity obbEntityB : obbEntities)
        {
            if (obbEntityA == obbEntityB) continue;
            auto &colliderB = registry.GetComponent<Components::OBBCollider>(obbEntityB);
            auto &transformB = registry.GetComponent<Components::Transform>(obbEntityB);
            auto worldOBBB = colliderB.GetWorldOBB(transformB);

            const bool collision = CheckCollision(worldOBBA, worldOBBB);

#if ENABLE_LOG
            if (collision)
                std::cout << std::format("OBB vs OBB Collision by entities {} and {}\n", obbEntityA, obbEntityB);
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

bool ECS::Systems::CollisionSystem::CheckCollision(const Components::OBBCollider &a, const Components::OBBCollider &b)
{
    glm::vec3 aAxes[3];
    glm::mat3 aRotMat = glm::mat3_cast(a.rotation);
    aAxes[0] = aRotMat[0];
    aAxes[1] = aRotMat[1];
    aAxes[2] = aRotMat[2];

    glm::vec3 bAxes[3];
    glm::mat3 bRotMat = glm::mat3_cast(b.rotation);
    bAxes[0] = bRotMat[0];
    bAxes[1] = bRotMat[1];
    bAxes[2] = bRotMat[2];

    const glm::vec3 T = b.center - a.center;

    auto checkSeparatingAxis = [&](const glm::vec3 &axis) -> bool
    {
        if (glm::dot(axis, axis) < 0.0001f) return false;

        const float aProjectedRadious =
            a.halfExtents.x * std::abs(glm::dot(axis, aAxes[0])) +
            a.halfExtents.y * std::abs(glm::dot(axis, aAxes[1])) +
            a.halfExtents.z * std::abs(glm::dot(axis, aAxes[2]));

        const float bProjectedRadious =
            b.halfExtents.x * std::abs(glm::dot(axis, bAxes[0])) +
            b.halfExtents.y * std::abs(glm::dot(axis, bAxes[1])) +
            b.halfExtents.z * std::abs(glm::dot(axis, bAxes[2]));

        const float distance = std::abs(glm::dot(T, axis));

        return distance > aProjectedRadious + bProjectedRadious;
    };

    if (checkSeparatingAxis(aAxes[0])) return false;
    if (checkSeparatingAxis(aAxes[1])) return false;
    if (checkSeparatingAxis(aAxes[2])) return false;

    if (checkSeparatingAxis(bAxes[0])) return false;
    if (checkSeparatingAxis(bAxes[1])) return false;
    if (checkSeparatingAxis(bAxes[2])) return false;

    for (const glm::vec3 &aAxe : aAxes)
        for (const glm::vec3 &bAxe : bAxes)
            if (checkSeparatingAxis(glm::cross(aAxe, bAxe))) return false;

    return true;
}