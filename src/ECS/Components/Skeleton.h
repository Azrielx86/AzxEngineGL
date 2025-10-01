//
// Created by edgar on 9/30/25.
//

#ifndef OPENGLPLAYGROUND_SKELETON_H
#define OPENGLPLAYGROUND_SKELETON_H
#include "Model.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>
#include <vector>

struct Skeleton
{
    std::vector<glm::mat4> finalBonesMatrices;
    std::unordered_map<std::string, BoneInfo, string_hash> boneInfoMap;
    int boneCount = 0;
};

#endif // OPENGLPLAYGROUND_SKELETON_H
