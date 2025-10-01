//
// Created by edgar on 9/30/25.
//

#ifndef OPENGLPLAYGROUND_SKINNEDANIMATION_H
#define OPENGLPLAYGROUND_SKINNEDANIMATION_H
#include "Bone.h"
#include "Utils.h"

#include "BoneInfo.h"
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class Model;

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class SkinnedAnimation
{
    float duration = 0;
    int ticksPerSecond = 25;
    std::vector<Bone> m_Bones;
    AssimpNodeData rootNode;
    std::unordered_map<std::string, BoneInfo, string_hash> boneInfoMap;

    void ReadMissingBones(const aiAnimation *animation, Model &model) const;
    static void ReadHierarchyData(AssimpNodeData &dest, const aiNode *src);

  public:
    SkinnedAnimation() = default;
    SkinnedAnimation(const aiAnimation *animation, const aiScene *scene, Model &model);
    Bone *FindBone(const std::string &name);

    [[nodiscard]] int GetTicksPerSecond() const { return ticksPerSecond; }

    [[nodiscard]] float GetDuration() const { return duration; }

    const AssimpNodeData &GetRootNode() { return rootNode; }

    [[nodiscard]] std::unordered_map<std::string, BoneInfo, string_hash> &GetBoneIDMap() { return boneInfoMap; }
};

#endif // OPENGLPLAYGROUND_SKINNEDANIMATION_H
