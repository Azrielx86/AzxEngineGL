//
// Created by kaguya on 3/13/25.
//

#include "Framebuffer.h"

// clang-format off
float Framebuffer::quadVertices[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f,  -1.0f,  0.0f, 0.0f,
	 1.0f,  -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f,  -1.0f,  1.0f, 0.0f
};
// clang-format on

int Framebuffer::GetResolutionValue(const int value, const float aspect_ratio, const ResolutionReference reference)
{
    return static_cast<int>(std::ceil((reference == HEIGHT)  ? static_cast<float>(value) * aspect_ratio
                                      : (reference == WIDTH) ? static_cast<float>(value) / aspect_ratio
                                                             : static_cast<float>(value)));
}

Framebuffer::Framebuffer(Shader &shader, const int width, const int height)
{
    this->shader = std::make_unique<Shader>(shader);

    this->width = width;
    this->height = height;

    CreateFramebuffer(width, height);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void *>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(sizeof(float) * 2));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Framebuffer::~Framebuffer()
{
    DestroyFramebuffer();
}

void Framebuffer::BeginRender() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, width, height);
}

void Framebuffer::EnableMainFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Framebuffer::RenderQuad() const
{
    shader->Use();
    glBindVertexArray(vao);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Framebuffer::CreateFramebuffer(const int vwidth, const int vheight)
{
    if (resReference == HEIGHT)
    {
        height = maxHeight;
        width = GetResolutionValue(maxHeight, static_cast<float>(vwidth) / static_cast<float>(vheight), HEIGHT);
    }
    else if (resReference == WIDTH)
    {
        width = maxWidth;
        height = GetResolutionValue(maxWidth, static_cast<float>(vwidth) / static_cast<float>(vheight), WIDTH);
    }
    else
    {
        height = vheight;
        width = vwidth;
    }

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, renderFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, renderFilter);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::DestroyFramebuffer() const
{
    if (fbo != 0)
        glDeleteFramebuffers(1, &fbo);

    if (textureId != 0)
        glDeleteTextures(1, &textureId);

    if (rbo != 0)
        glDeleteRenderbuffers(1, &rbo);
}

void Framebuffer::ReloadShader() const
{
    shader->ReloadShader();
}

void Framebuffer::SetMaxResolution(const ResolutionReference reference, const int value)
{
    if (reference == HEIGHT)
        maxHeight = value;
    else if (reference == WIDTH)
        maxWidth = value;
    else
        maxWidth = maxHeight = 0;

    resReference = reference;
}

void Framebuffer::SetRenderFilter(GLint filter)
{
    this->renderFilter = filter;
}

float Framebuffer::GetAspect() const
{
    return static_cast<float>(width) / static_cast<float>(height);
}
