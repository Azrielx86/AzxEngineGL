//
// Created by tohka on 8/26/25.
//

#include "Entity.h"

namespace World
{
unsigned int Entity::entityCount = 0;

Entity::Entity(const unsigned int id)
{
	entityId = id;
}

Entity Entity::CreateEntity()
{
	const Entity entity(entityCount);
	entityCount++;
	return entity;
}
} // namespace World
