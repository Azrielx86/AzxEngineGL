//
// Created by tohka on 9/9/25.
//

#include "Registry.h"

#include <ranges>

namespace ECS
{
Entity Registry::CreateEntity()
{
    return nextEntity++;
}

void Registry::DestroyEntity(Entity entity)
{
    if (!entitySignature.contains(entity))
    {
        std::cout << std::format("Entity {} does not exists!\n", entity);
        return;
    }

    for (auto componentIndex : entitySignature[entity])
    {
        const auto component = componentArrays[componentIndex];
        component->RemoveEntity(entity);
    }

    entitySignature.erase(entity);
}

void Registry::Reset()
{
    for (const auto &arr : componentArrays | std::views::values)
        arr->Reset();
    nextEntity = 1;
    entitySignature.clear();
}

size_t Registry::GetEntityCount() const { return entitySignature.size(); }
} // namespace ECS