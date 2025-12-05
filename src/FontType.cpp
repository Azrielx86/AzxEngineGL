//
// Created by tohka on 12/5/25.
//

#include "FontType.h"

#include <format>
#include <stdexcept>
#include <utility>

const char *FontType::vertexShader = ""
                                     "#version 410 core\n"
                                     "in vec4 in_Position;\n"
                                     "out vec2 texCoords;\n"
                                     "void main(void) {\n"
                                     "    gl_Position = vec4(in_Position.xy, 0, 1);\n"
                                     "    texCoords = in_Position.zw;\n"
                                     "}\n";

const char *FontType::fragmentShader = ""
                                       "#version 410 core\n"
                                       "precision highp float;\n"
                                       "uniform sampler2D tex;\n"
                                       "uniform vec4 color;\n"
                                       "in vec2 texCoords;\n"
                                       "out vec4 fragColor;\n"
                                       "void main(void) {\n"
                                       "    fragColor = vec4(1, 1, 1, texture(tex, texCoords).r) * color;\n"
                                       "}\n";

void FontType::RenderText(const std::string &str, const FT_Face fc, float x, float y, const float sx, const float sy) // NOLINT(*-misplaced-const)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    const FT_GlyphSlot glyph = fc->glyph; // NOLINT(*-misplaced-const)

    const bool isCullEnabled = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    for (const char &c : str)
    {
        if (FT_Load_Char(fc, c, FT_LOAD_RENDER) != 0)
            continue;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, static_cast<GLsizei>(glyph->bitmap.width),
                     static_cast<GLsizei>(glyph->bitmap.rows), 0, GL_RED, GL_UNSIGNED_BYTE,
                     glyph->bitmap.buffer);

        const float vx = x + static_cast<float>(glyph->bitmap_left) * sx;
        const float vy = y - static_cast<float>(glyph->bitmap_top) * sy;
        const float w = static_cast<float>(glyph->bitmap.width) * sx;
        const float h = static_cast<float>(glyph->bitmap.rows) * sy;

        struct
        {
            float x, y, s, t;
        } data[6] = {{vx, vy, 0, 0}, {vx, vy - h, 0, 1}, {vx + w, vy, 1, 0}, {vx + w, vy, 1, 0}, {vx, vy - h, 0, 1}, {vx + w, vy - h, 1, 1}};

        glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), data, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += static_cast<float>((glyph->advance.x >> 6)) * sx;
        y += static_cast<float>((glyph->advance.y >> 6)) * sy;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    if (isCullEnabled)
        glEnable(GL_CULL_FACE);
}

FontType::FontType(const float screenWidth, const float screenHeight, std::string fontPath, const float scaleFactor) : fontPath(std::move(fontPath))
{
    scaleX = scaleFactor / screenWidth;
    scaleY = scaleFactor / screenHeight;
}

void FontType::Init()
{
    if (FT_Init_FreeType(&ft_lib) != 0)
        throw std::runtime_error("Font cannot be initialized!");

    if (FT_New_Face(ft_lib, fontPath.c_str(), 0, &face) != 0)
        throw std::runtime_error(std::format("Can't load {} font.", fontPath));

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenTextures(1, &texture);
    glGenSamplers(1, &sampler);

    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &FontType::vertexShader, nullptr);
    glCompileShader(vs);

    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &FontType::fragmentShader, nullptr);
    glCompileShader(fs);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glUseProgram(program);
    glBindAttribLocation(program, 0, "in_position");

    texUniform = glGetUniformLocation(program, "tex");
    colorUniform = glGetUniformLocation(program, "color");
}

void FontType::Render(const float x, const float y, const std::string &str) const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindSampler(0, sampler);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glUseProgram(program);

    glUniform4f(static_cast<GLint>(colorUniform), 1.0f, 1.0f, 1.0f, 1.0f);

    glUniform1i(static_cast<GLint>(texUniform), 0);

    FT_Set_Pixel_Sizes(face, 0, 128);

    glEnable(GL_BLEND);
    RenderText(str, face, x, y, scaleX, scaleY);

    glBindSampler(0, 0);
    glDisable(GL_BLEND);
}