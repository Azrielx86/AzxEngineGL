//
// Created by tohka on 9/9/25.
//

#ifndef OPENGLPLAYGROUND_ICOMPONENTARRAY_H
#define OPENGLPLAYGROUND_ICOMPONENTARRAY_H
#include "Types.h"

namespace ECS
{
class IComponentArray
{
  public:
    virtual ~IComponentArray() = default;
    virtual void RemoveEntity(Entity entity) = 0;
    [[nodiscard]] virtual bool HasComponent(Entity entity) const = 0;
};
} // namespace ECS

#endif // OPENGLPLAYGROUND_ICOMPONENTARRAY_H
