//
// Created by kaguya on 7/20/24.
//

#ifndef SHADERPLAYGROUND_MODEL_H
#define SHADERPLAYGROUND_MODEL_H

#include "BoneInfo.h"
#include "Mesh.h"
#include "Resources/Material.h"
#include "SkinnedAnimation.h"
#include "Utils.h"
#include <assimp/scene.h>
#include <vector>

class Model
{
  public:
    explicit Model(const char *path);
    void Render(Shader &shader);

    [[nodiscard]] std::unordered_map<std::string, BoneInfo, string_hash> &GetBoneInfoMap() { return boneInfoMap; }

    [[nodiscard]] int GetBoneCount() const { return boneCounter; }

    void IncrementBoneCount() { boneCounter++; }

    void AddBoneInfo(const std::string &name, int id, const glm::mat4 &offset);

    [[nodiscard]] SkinnedAnimation *GetAnimation(int index);

    [[nodiscard]] bool HasTransparency() const { return hasTransparency; }

  private:
    std::vector<Mesh> meshes;
    std::unordered_map<std::string, BoneInfo, string_hash> boneInfoMap;
    int boneCounter{};
    glm::mat4 globalInverseTransform{};
    std::string modelPath{};
    // const aiScene *scene{};
    std::vector<SkinnedAnimation> animations;
    bool hasTransparency = false;

    void LoadModel(const char *path);
    void LoadNode(const aiNode *pNode, const aiScene *pScene);
    void LoadMesh(const aiMesh *mesh, [[maybe_unused]] [[maybe_unused]] [[maybe_unused]] const aiScene *aiScene);
    static std::vector<std::shared_ptr<Resources::Texture>> LoadMaterialTextures(const aiMaterial *material, aiTextureType type);
    static Resources::Material LoadMaterial(const aiMaterial *assimpMaterial);
    void ExtractBoneWeightVertices(std::vector<Vertex> &vertices, const aiMesh *mesh, const aiScene *scene);
    static void SetVertexBoneData(Vertex &vertex, int boneId, float weight);
};

#endif // SHADERPLAYGROUND_MODEL_H
