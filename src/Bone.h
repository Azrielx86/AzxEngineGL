//
// Created by tohka on 9/10/25.
//

#ifndef OPENGLPLAYGROUND_BONE_H
#define OPENGLPLAYGROUND_BONE_H

#include <assimp/anim.h>
#include <glm/glm.hpp>
#include <string>

class Bone
{
    glm::mat4 localTransform = glm::mat4 (1.0f);
    std::string name;
    int id;
public:
    Bone(std::string  name, const aiNodeAnim* channel);
    void Update(float time);
    int GetPositionIndex(float time);
    int GetRotationIndex(float time);
    int GetScaleIndex(float time);
};

#endif // OPENGLPLAYGROUND_BONE_H
