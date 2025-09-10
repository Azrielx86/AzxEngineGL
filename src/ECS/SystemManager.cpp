//
// Created by tohka on 9/10/25.
//

#include "SystemManager.h"

namespace ECS
{
void SystemManager::UpdateAll(Registry &registry, const float deltaTime) const
{
    for (const auto &system : systems)
        system->Update(registry, deltaTime);
}
} // namespace ECS