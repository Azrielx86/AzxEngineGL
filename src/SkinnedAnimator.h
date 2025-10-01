//
// Created by edgar on 10/1/25.
//

#ifndef OPENGLPLAYGROUND_SKINNEDANIMATOR_H
#define OPENGLPLAYGROUND_SKINNEDANIMATOR_H
#include "SkinnedAnimation.h"

class SkinnedAnimator
{
    std::vector<glm::mat4> finalBoneMatrices;
    SkinnedAnimation *currentAnim;
    float currentTime;
    float deltaTime;

  public:
    SkinnedAnimator();
    explicit SkinnedAnimator(SkinnedAnimation *animation);

    void UpdateAnimation(float dt);
    void PlayAnimation(SkinnedAnimation *animation);
    void CalculateBoneTransform(const AssimpNodeData *node, const glm::mat4 &parentTransform);

    [[nodiscard]] std::vector<glm::mat4> &GetFinalBoneMatrices() { return finalBoneMatrices; };
};

#endif // OPENGLPLAYGROUND_SKINNEDANIMATOR_H
