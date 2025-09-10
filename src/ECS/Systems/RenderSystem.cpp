//
// Created by tohka on 9/10/25.
//

#include "RenderSystem.h"

#include "ECS/Components/MeshRenderer.h"
#include "ECS/Components/Transform.h"

namespace ECS::Systems
{
glm::mat4 RenderSystem::GetWorldTransform(Registry &registry, const Entity entity)
{
    auto &[translation, rotation, scale, parent] = registry.GetComponent<Components::Transform>(entity);

    const auto localTransform = glm::translate(glm::mat4(1.0f), translation) *
                                glm::mat4_cast(rotation) *
                                glm::scale(glm::mat4(1.0f), scale);

    if (parent != NullEntity)
        return GetWorldTransform(registry, entity) * localTransform;

    return localTransform;
}

void RenderSystem::Update(Registry &registry, [[maybe_unused]] float deltaTime)
{
    for (const Entity &entity : registry.View<Components::MeshRenderer, Components::Transform>())
    {
        auto &[model, shader] = registry.GetComponent<Components::MeshRenderer>(entity);
        const auto uModel = shader->GetUniformLocation("model");
        shader->Set<4, 4>(uModel, GetWorldTransform(registry, entity));
        model->Render(*shader);
    }
}

} // namespace ECS::Systems
