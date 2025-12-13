//
// Created by tohka on 9/10/25.
//

#ifndef OPENGLPLAYGROUND_SYSTEMMANAGER_H
#define OPENGLPLAYGROUND_SYSTEMMANAGER_H
#include "ISystem.h"

#include <vector>

namespace ECS
{

class SystemManager
{
    std::vector<std::shared_ptr<ISystem>> systems;

  public:
    template <typename T>
    void RegisterSystem();

    template <typename T>
    [[nodiscard]] std::shared_ptr<T> GetSystem() const;

    void UpdateAll(Registry &registry, float deltaTime) const;
};

template <typename T>
void SystemManager::RegisterSystem()
{
    auto sys = std::make_shared<T>();
    systems.push_back(static_cast<std::shared_ptr<ISystem>>(sys));
}

template <typename T>
std::shared_ptr<T> SystemManager::GetSystem() const
{
    for (const auto& system : systems)
    {
        if (auto castedSystem = std::dynamic_pointer_cast<T>(system))
            return castedSystem;
    }
    return nullptr;
}
} // namespace ECS

#endif // OPENGLPLAYGROUND_SYSTEMMANAGER_H
