//
// Created by tohka on 8/26/25.
//

#ifndef SHADERPLAYGROUND_ENTITY_H
#define SHADERPLAYGROUND_ENTITY_H

namespace World
{
class Entity
{
	static unsigned int entityCount;

	unsigned int entityId = 0;
	explicit Entity(unsigned int id);

  public:
	static Entity CreateEntity();
};
} // namespace World

#endif // SHADERPLAYGROUND_ENTITY_H
