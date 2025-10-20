//
// Created by tohka on 9/10/25.
//

#include "RenderSystem.h"

#include "ECS/Components/MeshRenderer.h"
#include "ECS/Components/Transform.h"

namespace ECS::Systems
{
glm::mat4 RenderSystem::GetWorldTransform(Registry &registry, const Entity entity) // NOLINT(*-no-recursion)
{
    auto &[translation, rotation, scale, parent] = registry.GetComponent<Components::Transform>(entity);

    const auto localTransform = glm::translate(glm::mat4(1.0f), translation) *
                                glm::mat4_cast(rotation) *
                                glm::scale(glm::mat4(1.0f), scale);

    if (parent != NullEntity)
        return GetWorldTransform(registry, parent) * localTransform;

    return localTransform;
}

void RenderSystem::Update(Registry &registry, [[maybe_unused]] float deltaTime)
{
    // TODO : Render in order with the camera position (Needs to create a camera entity).

    std::vector<Entity> opaqueEntities;
    std::vector<Entity> transparentEntities;

    for (const Entity &entity : registry.View<Components::MeshRenderer, Components::Transform>())
    {
        if (registry.GetComponent<Components::MeshRenderer>(entity).model->HasTransparency())
            transparentEntities.push_back(entity);
        else
            opaqueEntities.push_back(entity);
    }

    for (const Entity& entity : opaqueEntities)
        RenderEntity(registry, entity);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (const Entity& entity : transparentEntities)
        RenderEntity(registry, entity);
    glDisable(GL_BLEND);
}

void RenderSystem::RenderEntity(Registry &registry, const Entity entity)
{
    auto &[model, shader] = registry.GetComponent<Components::MeshRenderer>(entity);
    const auto uModel = shader->GetUniformLocation("model");
    shader->Set<4, 4>(uModel, GetWorldTransform(registry, entity));
    model->Render(*shader);
}
} // namespace ECS::Systems
