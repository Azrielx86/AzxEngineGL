//
// Created by tohka on 9/9/25.
//

#ifndef OPENGLPLAYGROUND_TYPES_H
#define OPENGLPLAYGROUND_TYPES_H

#include <cstdint>

namespace ECS
{
using Entity = std::uint32_t;

constexpr Entity MaxEntities = 10'000;

constexpr Entity NullEntity = 0;
} // namespace ECS

#endif // OPENGLPLAYGROUND_TYPES_H
