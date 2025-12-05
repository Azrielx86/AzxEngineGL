//
// Created by tohka on 12/5/25.
//

#ifndef PROYECTOFINAL_CGA_FONTTYPE_H
#define PROYECTOFINAL_CGA_FONTTYPE_H

#include <GL/glew.h>
#include <freetype/freetype.h>
#include <string>

class FontType
{
    static const char *fragmentShader;
    static const char *vertexShader;

    float scaleX = 0.0f;
    float scaleY = 0.0f;
    GLuint texture = 0;
    GLuint sampler = 0;
    GLuint vbo = 0, vao = 0;
    GLuint vs = 0, fs = 0, program = 0;
    GLuint texUniform = 0, colorUniform = 0;
    std::string fontPath;
    FT_Library ft_lib{nullptr};
    FT_Face face{nullptr};

    static void RenderText(const std::string &str, FT_Face fc, float x, float y, float sx, float sy);

  public:
    FontType(float screenWidth, float screenHeight, std::string fontPath, float scaleFactor = 1.0f);

    void Init();
    void Render(float x, float y, const std::string &str) const;
};

#endif // PROYECTOFINAL_CGA_FONTTYPE_H
