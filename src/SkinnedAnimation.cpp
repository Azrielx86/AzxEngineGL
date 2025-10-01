//
// Created by edgar on 9/30/25.
//

#include "SkinnedAnimation.h"
#include "Model.h"

#include "Assimp2Glm.h"

SkinnedAnimation::SkinnedAnimation(const aiAnimation *animation, const aiScene *scene, Model &model)
{
    duration = static_cast<float>(animation->mDuration);
    ticksPerSecond = static_cast<int>(animation->mTicksPerSecond);

    if (ticksPerSecond == 0) ticksPerSecond = 25;

    ReadHierarchyData(rootNode, scene->mRootNode);
    ReadMissingBones(animation, model);

    auto &modelBoneInfoMap = model.GetBoneInfoMap();

    for (unsigned int i = 0; i < animation->mNumChannels; i++)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.C_Str();

        if (!modelBoneInfoMap.contains(boneName))
        {
            modelBoneInfoMap[boneName].id = model.GetBoneCount();
            model.IncrementBoneCount();
        }

        m_Bones.emplace_back(boneName, modelBoneInfoMap[boneName].id, channel);
    }

    boneInfoMap = modelBoneInfoMap;
}

void SkinnedAnimation::ReadMissingBones(const aiAnimation *animation, Model &model) const
{
    for (unsigned int i = 0; i < animation->mNumChannels; ++i)
    {
        const auto channel = animation->mChannels[i];
        if (const std::string boneName = channel->mNodeName.C_Str(); !model.GetBoneInfoMap().contains(boneName))
            model.AddBoneInfo(boneName, model.GetBoneCount(), glm::mat4(1.0f));
    }
}

void SkinnedAnimation::ReadHierarchyData(AssimpNodeData &dest, const aiNode *src) // NOLINT(*-no-recursion)
{
    dest.name = src->mName.data;
    dest.transformation = Assimp2Glm::ConvertMatrix4x4(src->mTransformation);
    dest.childrenCount = static_cast<int>(src->mNumChildren);

    for (unsigned int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHierarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}

Bone *SkinnedAnimation::FindBone(const std::string &name)
{
    const auto iter = std::ranges::find_if(m_Bones, [&name](const Bone &bone) -> bool
                                           {
                                               return bone.GetBoneName() == name;
                                           });
    if (iter == m_Bones.end()) return nullptr;
    return &(*iter);
}