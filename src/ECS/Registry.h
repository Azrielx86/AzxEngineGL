//
// Created by tohka on 9/9/25.
//

#ifndef OPENGLPLAYGROUND_REGISTRY_H
#define OPENGLPLAYGROUND_REGISTRY_H

#include "ComponentArray.h"
#include "Types.h"
#include <memory>
#include <set>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace ECS
{

class Registry
{
    Entity nextEntity = 1;
    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> componentArrays{};
    std::unordered_map<Entity, std::set<std::type_index>> entitySignature{};

    template <typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray();

  public:
    [[nodiscard]] Entity CreateEntity();

    template <typename T>
    void RegisterComponent();

    template <typename T>
    void AddComponent(Entity entity, T component);

    template <typename T>
    [[nodiscard]] T &GetComponent(Entity entity);

    template <typename... Components>
    [[nodiscard]] std::vector<Entity> View();

    template <typename T>
    [[nodiscard]] bool HasComponent(Entity entity);
};

template <typename T>
std::shared_ptr<ComponentArray<T>> Registry::GetComponentArray()
{
    return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeid(T)]);
}

template <typename T>
void Registry::RegisterComponent()
{
    componentArrays[typeid(T)] = static_cast<std::shared_ptr<IComponentArray>>(std::make_shared<ComponentArray<T>>());
}

template <typename T>
void Registry::AddComponent(Entity entity, T component)
{
    std::shared_ptr<ComponentArray<T>> array = GetComponentArray<T>();
    array->Insert(entity, component);
    entitySignature[entity].insert(typeid(T));
}

template <typename T>
T &Registry::GetComponent(Entity entity)
{
    return GetComponentArray<T>()->GetData(entity);
}

template <typename... Components>
std::vector<Entity> Registry::View()
{
    std::vector<Entity> entities;
    const std::set<std::type_index> requiredComponents = {typeid(Components)...};

    for (auto const &[entity, signature] : entitySignature)
    {
        bool matches = true;
        for (const auto required : requiredComponents)
        {
            if (!signature.contains(required))
            {
                matches = false;
                break;
            }
        }

        if (matches)
            entities.push_back(entity);
    }

    return entities;
}

template <typename T>
bool Registry::HasComponent(const Entity entity)
{
    return GetComponentArray<T>()->HasComponent(entity);
}

} // namespace ECS

#endif // OPENGLPLAYGROUND_REGISTRY_H
