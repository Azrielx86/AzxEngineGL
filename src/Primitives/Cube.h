//
// Created by tohka on 11/6/25.
//

#ifndef OPENGLPLAYGROUND_CUBE_H
#define OPENGLPLAYGROUND_CUBE_H
#include "AbstractPrimitive.h"

namespace Primitives
{
class Cube final : AbstractPrimitive
{
  public:
    void Init() override;
    ~Cube() override;
    void Render() override;
};
} // namespace Primitives

#endif // OPENGLPLAYGROUND_CUBE_H
