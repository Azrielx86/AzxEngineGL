//
// Created by tohka on 9/9/25.
//

#ifndef OPENGLPLAYGROUND_ISYSTEM_H
#define OPENGLPLAYGROUND_ISYSTEM_H
#include "Registry.h"

namespace ECS
{
class ISystem
{
  public:
	virtual ~ISystem() = default;
	virtual void Update(Registry &registry, float deltaTime) = 0;
};
} // namespace ECS

#endif // OPENGLPLAYGROUND_ISYSTEM_H
