//
// Created by kaguya on 3/13/25.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "Shader.h"

#include <memory>

enum ResolutionReference
{
    NONE,
    WIDTH,
    HEIGHT
};

class Framebuffer
{
    unsigned int fbo = 0;
    unsigned int textureId = 0;
    unsigned int rbo = 0;
    unsigned int vao = 0;
    unsigned int vbo = 0;
    std::unique_ptr<Shader> shader;
    static float quadVertices[];
    int width = 0;
    int height = 0;
    int maxWidth = 0;
    int maxHeight = 0;
    ResolutionReference resReference = NONE;
    GLint renderFilter = GL_LINEAR;
    [[nodiscard]] static int GetResolutionValue(int value, float aspect_ratio, ResolutionReference reference);

  public:
    Framebuffer(Shader &shader, int width, int height);
    ~Framebuffer();
    void BeginRender() const;
    static void EnableMainFramebuffer();
    void RenderQuad() const;
    void CreateFramebuffer(int vwidth, int vheight);
    void DestroyFramebuffer() const;
    void ReloadShader() const;
    void SetMaxResolution(ResolutionReference reference, int value);
    void SetRenderFilter(GLint filter);
    [[nodiscard]] float GetAspect() const;
};

#endif // FRAMEBUFFER_H
