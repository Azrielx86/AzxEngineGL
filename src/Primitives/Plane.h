//
// Created by tohka on 11/6/25.
//

#ifndef OPENGLPLAYGROUND_PLANE_H
#define OPENGLPLAYGROUND_PLANE_H
#include "AbstractPrimitive.h"

namespace Primitives
{
class Plane final : AbstractPrimitive
{
  public:
    void Init() override;
    void Render() override;
    ~Plane() override;
};
} // namespace Primitives

#endif // OPENGLPLAYGROUND_PLANE_H
