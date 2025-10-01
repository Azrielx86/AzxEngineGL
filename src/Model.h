//
// Created by kaguya on 7/20/24.
//

#ifndef SHADERPLAYGROUND_MODEL_H
#define SHADERPLAYGROUND_MODEL_H

#include "Mesh.h"
#include "Resources/Material.h"
#include "Utils.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

struct BoneInfo
{
    int id;
    glm::mat4 offset;
};

class Model
{
  public:
    explicit Model(const char *path);
    void Render(Shader &shader);

  private:
    std::vector<Mesh> meshes;
    std::unordered_map<std::string, BoneInfo, string_hash> boneInfoMap;
    int m_BoneCounter{};
    glm::mat4 globalInverseTransform{};
    std::string modelPath{};
    // const aiScene* scene;
    int animationIndex{};

    void LoadModel(const char *path);
    void LoadNode(const aiNode *pNode, const aiScene *pScene);
    void LoadMesh(const aiMesh *mesh, [[maybe_unused]] [[maybe_unused]] [[maybe_unused]] const aiScene *aiScene);
    static std::vector<std::shared_ptr<Resources::Texture>> LoadMaterialTextures(const aiMaterial *material, aiTextureType type);
    static Resources::Material LoadMaterial(const aiMaterial *assimpMaterial);
    void ExtractBoneWeightVertices(std::vector<Vertex> &vertices, const aiMesh *mesh, const aiScene *scene);
    static void SetVertexBoneData(Vertex& vertex, int boneId, float weight);
};

#endif // SHADERPLAYGROUND_MODEL_H
