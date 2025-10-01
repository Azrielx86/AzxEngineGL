//
// Created by tohka on 9/10/25.
//

#ifndef OPENGLPLAYGROUND_ASSIMP3GLM_H
#define OPENGLPLAYGROUND_ASSIMP3GLM_H
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Assimp2Glm
{
  public:
    static glm::mat4 ConvertMatrix4x4(const aiMatrix4x4 &from);

    static glm::vec3 ConvertVector3(const aiVector3D &vec);

    static glm::quat ConvertQuaternion(const aiQuaternion &pOrientation);
};

#endif // OPENGLPLAYGROUND_ASSIMP3GLM_H
