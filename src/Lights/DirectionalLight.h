//
// Created by tohka on 11/22/25.
//

#ifndef PROYECTOFINAL_CGA_DIRECTIONALLIGHT_H
#define PROYECTOFINAL_CGA_DIRECTIONALLIGHT_H
#include <glm/glm.hpp>

namespace Lights
{
struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
} // namespace Lights

#endif // PROYECTOFINAL_CGA_DIRECTIONALLIGHT_H
