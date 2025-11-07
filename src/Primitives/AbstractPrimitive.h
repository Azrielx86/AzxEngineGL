//
// Created by tohka on 11/6/25.
//

#ifndef OPENGLPLAYGROUND_ABSTRACTPRIMITIVE_H
#define OPENGLPLAYGROUND_ABSTRACTPRIMITIVE_H
#include <GL/glew.h>

namespace Primitives
{
class AbstractPrimitive
{
  public:
    virtual ~AbstractPrimitive() = default;
    virtual void Init() = 0;
    virtual void Render();

  protected:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ibo = 0;
    GLint indexCount = 0;
};
} // namespace Primitives

#endif // OPENGLPLAYGROUND_ABSTRACTPRIMITIVE_H
