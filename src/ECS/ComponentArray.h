//
// Created by tohka on 9/9/25.
//

#ifndef OPENGLPLAYGROUND_COMPONENTARRAY_H
#define OPENGLPLAYGROUND_COMPONENTARRAY_H
#include "IComponentArray.h"
#include "Types.h"
#include <array>
#include <iostream>
#include <unordered_map>

namespace ECS
{

template <typename T>
class ComponentArray final : public IComponentArray
{
	std::array<T, MaxEntities> componentArray;
	std::unordered_map<Entity, size_t> entityToIndex;
	std::unordered_map<size_t, Entity> indexToEntity;
	size_t size = 0;

  public:
	void Insert(Entity entity, T component);

	T &GetData(Entity entity);

    bool HasComponent(Entity entity) const;

	~ComponentArray() override;
};

template <typename T>
void ComponentArray<T>::Insert(const Entity entity, T component)
{
	if (entityToIndex.contains(entity))
	{
		std::cout << "Entity with ID " << entity << " already have a " << typeid(T).name() << " component registered.\n";
		return;
	}

	size_t newIndex = size;
	entityToIndex[entity] = newIndex;
	indexToEntity[newIndex] = entity;
	componentArray[newIndex] = component;
	size++;
}

template <typename T>
T &ComponentArray<T>::GetData(const Entity entity)
{
	return componentArray[entityToIndex[entity]];
}

template <typename T>
bool ComponentArray<T>::HasComponent(const Entity entity) const
{
    return entityToIndex.contains(entity);
}

template <typename T>
ComponentArray<T>::~ComponentArray() = default;

} // namespace ECS
#endif // OPENGLPLAYGROUND_COMPONENTARRAY_H
