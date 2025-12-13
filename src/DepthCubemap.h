//
// Created by tohka on 7/28/25.
//

#ifndef DEPTHCUBEMAP_H
#define DEPTHCUBEMAP_H

#include <GL/glew.h>

class DepthCubemap {
public:
    DepthCubemap();
    ~DepthCubemap();

    void Init(unsigned int width, unsigned int height);
    void Bind() const;
    void Unbind() const;
    GLuint GetDepthMap() const { return depthMap; }

private:
    GLuint FBO;
    GLuint depthMap;
    unsigned int shadowWidth, shadowHeight;
};

#endif //DEPTHCUBEMAP_H
