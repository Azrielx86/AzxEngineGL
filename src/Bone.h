//
// Created by tohka on 9/10/25.
//

#ifndef OPENGLPLAYGROUND_BONE_H
#define OPENGLPLAYGROUND_BONE_H

#include <assimp/anim.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};

class Bone
{
    glm::mat4 localTransform = glm::mat4(1.0f);
    std::string name;
    int id;
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;
    unsigned int numPositions;
    unsigned int numRotations;
    unsigned int numScalings;

    static float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float time);
    [[nodiscard]] glm::mat4 InterpolatePosition(float time) const;
    [[nodiscard]] glm::mat4 InterpolateRotation(float time) const;
    [[nodiscard]] glm::mat4 InterpolateScaling(float time) const;

  public:
    Bone(std::string name, int id, const aiNodeAnim *channel);
    void Update(float time);
    [[nodiscard]] unsigned int GetPositionIndex(float time) const;
    [[nodiscard]] unsigned int GetRotationIndex(float time) const;
    [[nodiscard]] unsigned int GetScaleIndex(float time) const;

    [[nodiscard]] glm::mat4 GetLocalTransform() const { return localTransform; }

    [[nodiscard]] std::string GetBoneName() const { return name; }

    [[nodiscard]] int GetBoneID() const { return id; }
};

#endif // OPENGLPLAYGROUND_BONE_H
