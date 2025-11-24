//
// Created by tohka on 9/9/25.
//

#include "Registry.h"

namespace ECS
{
Entity Registry::CreateEntity()
{
    return nextEntity++;
}

size_t Registry::GetEntityCount() const { return entitySignature.size(); }
} // namespace ECS