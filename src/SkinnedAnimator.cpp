//
// Created by edgar on 10/1/25.
//

#include "SkinnedAnimator.h"

SkinnedAnimator::SkinnedAnimator() : currentAnim(nullptr), currentTime(0), deltaTime(0)
{
    finalBoneMatrices.reserve(100);
    for (int i = 0; i < 100; i++)
        finalBoneMatrices.emplace_back(1.0f);
}

SkinnedAnimator::SkinnedAnimator(SkinnedAnimation *animation) : currentAnim(animation), currentTime(0), deltaTime(0)
{
    finalBoneMatrices.reserve(100);
    for (int i = 0; i < 100; i++)
        finalBoneMatrices.emplace_back(1.0f);
}

void SkinnedAnimator::UpdateAnimation(const float dt)
{
    deltaTime = dt;
    if (currentAnim)
    {
        currentTime += static_cast<float>(currentAnim->GetTicksPerSecond()) * deltaTime;
        currentTime = static_cast<float>(fmod(static_cast<double>(currentTime), static_cast<double>(currentAnim->GetDuration())));
        CalculateBoneTransform(&currentAnim->GetRootNode(), glm::mat4(1.0f));
    }
}

void SkinnedAnimator::PlayAnimation(SkinnedAnimation *animation)
{
    currentAnim = animation;
    currentTime = 0.0f;
}

void SkinnedAnimator::CalculateBoneTransform(const AssimpNodeData *node, const glm::mat4 &parentTransform)
{
    const std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    if (Bone *bone = currentAnim->FindBone(nodeName))
    {
        bone->Update(currentTime);
        nodeTransform = bone->GetLocalTransform();
    }

    const glm::mat4 globalTransformation = parentTransform * nodeTransform;

    if (auto &boneInfoMap = currentAnim->GetBoneIDMap(); boneInfoMap.contains(nodeName))
    {
        const auto &[id, offset] = boneInfoMap[nodeName];
        finalBoneMatrices[id] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneTransform(&node->children[i], globalTransformation);
}