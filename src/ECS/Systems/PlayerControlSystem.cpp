//
// Created by tohka on 9/10/25.
//

#include "PlayerControlSystem.h"

#include "../../Input/Keyboard.h"
#include "ECS/Components/PlayerController.h"
#include "ECS/Components/Transform.h"
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

namespace ECS::Systems
{
void PlayerControlSystem::Update(Registry &registry, float deltaTime)
{
    const auto kb = Input::Keyboard::GetInstance();

    for (const Entity &entity : registry.View<Components::PlayerController, Components::Transform>())
    {
        auto &transform = registry.GetComponent<Components::Transform>(entity);
        const auto &control = registry.GetComponent<Components::PlayerController>(entity);

        float yawAmount = 0.0f;

        if (kb->GetKeyPress(GLFW_KEY_LEFT))
            yawAmount += control.turnSpeed * deltaTime;

        if (kb->GetKeyPress(GLFW_KEY_RIGHT))
            yawAmount -= control.turnSpeed * deltaTime;

        glm::quat yawRotation = glm::angleAxis(glm::radians(yawAmount), glm::vec3(0.0f, 1.0f, 0.0f));

        transform.rotation = yawRotation * transform.rotation;
        transform.rotation = glm::normalize(transform.rotation);

        glm::vec3 forwardDir = glm::normalize(transform.rotation * glm::vec3(0.0f, 0.0f, -1.0f));

        if (kb->GetKeyPress(GLFW_KEY_UP))
            transform.translation -= forwardDir * control.moveSpeed * deltaTime;

        if (kb->GetKeyPress(GLFW_KEY_DOWN))
            transform.translation += forwardDir * control.moveSpeed * deltaTime;
    }
}
} // namespace ECS::Systems