//
// Created by tohka on 7/28/25.
//

#ifndef DEPTHMAP_H
#define DEPTHMAP_H

#include <GL/glew.h>

class DepthMap {
public:
    DepthMap();
    ~DepthMap();

    void Init(unsigned int width, unsigned int height);
    void Bind() const;
    void Unbind() const;
    GLuint GetDepthMap() const { return depthMap; }

private:
    GLuint FBO;
    GLuint depthMap;
    unsigned int shadowWidth, shadowHeight;
};

#endif //DEPTHMAP_H
