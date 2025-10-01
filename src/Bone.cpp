//
// Created by tohka on 9/10/25.
//

#include "Bone.h"

#include "Assimp2Glm.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <utility>

Bone::Bone(std::string name, const int id, const aiNodeAnim *channel)
    : localTransform(1.0f), name(std::move(name)), id(id)
{
    numPositions = channel->mNumPositionKeys;
    positions.reserve(numPositions);
    for (unsigned int i = 0; i < numPositions; ++i)
    {
        const auto key = channel->mPositionKeys[i];
        positions.push_back({.position = Assimp2Glm::ConvertVector3(key.mValue),
                             .timeStamp = static_cast<float>(key.mTime)});
    }

    numRotations = channel->mNumRotationKeys;
    rotations.reserve(numRotations);
    for (unsigned int i = 0; i < numRotations; ++i)
    {
        const auto key = channel->mRotationKeys[i];
        rotations.push_back({.orientation = Assimp2Glm::ConvertQuaternion(key.mValue),
                             .timeStamp = static_cast<float>(key.mTime)});
    }

    numScalings = channel->mNumScalingKeys;
    scales.reserve(numScalings);
    for (unsigned int i = 0; i < numScalings; ++i)
    {
        const auto key = channel->mScalingKeys[i];
        scales.push_back({.scale = Assimp2Glm::ConvertVector3(key.mValue),
                          .timeStamp = static_cast<float>(key.mTime)});
    }
}

void Bone::Update(const float time)
{
    const glm::mat4 translation = InterpolatePosition(time);
    const glm::mat4 rotation = InterpolateRotation(time);
    const glm::mat4 scale = InterpolateScaling(time);
    localTransform = translation * rotation * scale;
}

unsigned int Bone::GetPositionIndex(const float time) const
{
    for (unsigned int i = 0; i < numPositions - 1; i++)
        if (time < positions[i + 1].timeStamp) return i;
    return 0;
}

unsigned int Bone::GetRotationIndex(const float time) const
{
    for (unsigned int i = 0; i < numRotations - 1; i++)
        if (time < rotations[i + 1].timeStamp) return i;
    return 0;
}

unsigned int Bone::GetScaleIndex(const float time) const
{
    for (unsigned int i = 0; i < numScalings - 1; i++)
        if (time < scales[i + 1].timeStamp) return i;
    return 0;
}

float Bone::GetScaleFactor(const float lastTimeStamp, const float nextTimeStamp, const float time)
{
    const float midWayLength = time - lastTimeStamp;
    const float framesDiff = nextTimeStamp - lastTimeStamp;
    return midWayLength / framesDiff;
}

glm::mat4 Bone::InterpolatePosition(const float time) const
{
    if (numPositions == 1) return glm::translate(glm::mat4(1.0f), positions[0].position);

    const unsigned int p0Index = GetPositionIndex(time);
    const unsigned int p1Index = p0Index + 1;
    const float scaleFactor = GetScaleFactor(positions[p0Index].timeStamp, positions[p1Index].timeStamp, time);
    const glm::vec3 finalPosition = glm::mix(positions[p0Index].position, positions[p1Index].position, scaleFactor);
    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(const float time) const
{
    if (numRotations == 1) return glm::toMat4(glm::normalize(rotations[0].orientation));

    const unsigned int p0Index = GetRotationIndex(time);
    const unsigned int p1Index = p0Index + 1;
    const float scaleFactor = GetScaleFactor(rotations[p0Index].timeStamp, rotations[p1Index].timeStamp, time);
    const glm::quat finalRotation = glm::slerp(rotations[p0Index].orientation, rotations[p1Index].orientation, scaleFactor);
    return glm::toMat4(glm::normalize(finalRotation));
}

glm::mat4 Bone::InterpolateScaling(const float time) const
{
    if (numScalings == 1) return glm::scale(glm::mat4(1.0f), scales[0].scale);
    const unsigned int p0Index = GetScaleIndex(time);
    const unsigned int p1Index = p0Index + 1;
    const float scaleFactor = GetScaleFactor(scales[p0Index].timeStamp, scales[p1Index].timeStamp, time);
    const glm::vec3 finalScale = glm::mix(scales[p0Index].scale, scales[p1Index].scale, scaleFactor);
    return glm::scale(glm::mat4(1.0f), finalScale);
}