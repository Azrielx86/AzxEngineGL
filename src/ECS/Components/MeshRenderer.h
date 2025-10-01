//
// Created by tohka on 9/9/25.
//

#ifndef OPENGLPLAYGROUND_MESHRENDERER_H
#define OPENGLPLAYGROUND_MESHRENDERER_H
#include "Model.h"

namespace ECS::Components
{
struct MeshRenderer
{
	Model* model;
    Shader* shader;
};
} // namespace ECS::Components

#endif // OPENGLPLAYGROUND_MESHRENDERER_H
