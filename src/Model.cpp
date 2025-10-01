//
// Created by kaguya on 7/20/24.
//

#include "Model.h"
#include "Assimp2Glm.h"
#include <iostream>
#include <regex>

Model::Model(const char *path)
{
    LoadModel(path);
    for (Mesh &mesh : meshes)
        mesh.Load();
}

void Model::LoadModel(const char *path)
{
    modelPath = path;
    auto importer = Assimp::Importer();
    const auto aiScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

    if (!aiScene || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode)
    {
        std::cerr << "No se pudo cargar el modelo: " << importer.GetErrorString() << "\n";
        return;
    }

    globalInverseTransform = Assimp2Glm::ConvertMatrix4x4(aiScene->mRootNode->mTransformation);
    globalInverseTransform = glm::inverse(globalInverseTransform);

    LoadNode(aiScene->mRootNode, aiScene);
}

void Model::LoadNode(const aiNode *pNode, const aiScene *pScene) // NOLINT(*-no-recursion)
{
    for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
        LoadMesh(pScene->mMeshes[pNode->mMeshes[i]], pScene);
    for (unsigned int i = 0; i < pNode->mNumChildren; i++)
        LoadNode(pNode->mChildren[i], pScene);
}

void Model::LoadMesh(const aiMesh *mesh, [[maybe_unused]] const aiScene *aiScene)
{
    std::vector<Vertex> vertices(mesh->mNumVertices);
    std::vector<unsigned int> faces;
    std::vector<std::shared_ptr<Resources::Texture>> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        auto meshVertex = mesh->mVertices[i];
        auto meshTangents = mesh->mTangents[i];
        auto meshBitangents = mesh->mBitangents[i];
        Vertex vertex{
            .position = {meshVertex.x, meshVertex.y, meshVertex.z},
            .normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z},
            .texCoords = {0.0f, 0.0f},
            .tangent = {meshTangents.x, meshTangents.y, meshTangents.z},
            .bitangent = {meshBitangents.x, meshBitangents.y, meshBitangents.z},
            .boneIds = {-1, -1, -1, -1},
            .weights = {0.0f, 0.0f, 0.0f, 0.0f}};
        if (mesh->mTextureCoords[0])
            vertex.texCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};

        vertices[i] = vertex;
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        auto face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            faces.push_back(face.mIndices[j]);
    }

    ExtractBoneWeightVertices(vertices, mesh, aiScene);

    Resources::Material material{};

    // if (mesh->mMaterialIndex >= 0)
    if (aiScene->mNumMaterials > 0)
    {
        const auto mat = aiScene->mMaterials[mesh->mMaterialIndex];
        auto diffuseMaps = LoadMaterialTextures(mat, aiTextureType_DIFFUSE);
        auto specularMaps = LoadMaterialTextures(mat, aiTextureType_SPECULAR);
        auto emissiveMaps = LoadMaterialTextures(mat, aiTextureType_EMISSIVE);
        auto normalMaps = LoadMaterialTextures(mat, aiTextureType_NORMALS);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        material = LoadMaterial(mat);
        material.textured = !textures.empty();
    }

    // auto nMesh = Mesh(vertices, faces, textures, material);
    // nMesh.Load();
    // meshes.push_back(nMesh);
    meshes.emplace_back(vertices, faces, textures, material);
}

void Model::Render(Shader &shader)
{
    for (Mesh &mesh : meshes)
    {
        const auto material = mesh.GetMaterial();
        shader.Set<3>("material.ambient", material->ambient);
        shader.Set<3>("material.diffuse", material->diffuse);
        shader.Set<3>("material.emissive", material->emissive);
        shader.Set<3>("material.specular", material->specular);
        shader.Set("material.shininess", material->shininess);
        shader.Set("material.textured", material->textured);
        mesh.Render(shader);
    }
}

std::vector<std::shared_ptr<Resources::Texture>> Model::LoadMaterialTextures(const aiMaterial *material, const aiTextureType type)
{
    std::vector<std::shared_ptr<Resources::Texture>> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
    {
        aiString path;
        material->GetTexture(type, i, &path);

        std::cmatch results;
        std::regex_search(path.C_Str(), results, std::regex(R"([^\/]+\.\w+$)"));

        if (results.empty())
        {
            std::cerr << std::format("No texture for: {}\n", path.C_Str());
            continue;
        }
        auto texName = results[0].str();
        auto texture = Resources::ResourceManager::GetInstance()->GetTexture(texName);
        texture->type = type;
        textures.push_back(texture);
    }
    return textures;
}

Resources::Material Model::LoadMaterial(const aiMaterial *assimpMaterial)
{
    Resources::Material mat{};
    aiColor3D color;
    aiColor3D diffuse;
    aiColor3D ambient;
    aiColor3D specular;
    aiColor3D emissive;

    assimpMaterial->Get(AI_MATKEY_BASE_COLOR, color);
    mat.baseColor = glm::vec3(color.r, color.g, color.b);

    assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    mat.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);

    assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    mat.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);

    assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    mat.specular = glm::vec3(specular.r, specular.g, specular.b);

    assimpMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
    mat.emissive = glm::vec3(emissive.r, emissive.g, emissive.b);

    assimpMaterial->Get(AI_MATKEY_SHININESS, mat.shininess);

    return mat;
}

void Model::ExtractBoneWeightVertices(std::vector<Vertex> &vertices, const aiMesh *mesh, [[maybe_unused]] const aiScene *scene)
{
    for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneId;

        if (std::string boneName = mesh->mBones[boneIndex]->mName.C_Str(); !boneInfoMap.contains(boneName))
        {
            BoneInfo boneInfo{};
            boneInfo.id = m_BoneCounter;
            boneInfo.offset = Assimp2Glm::ConvertMatrix4x4(mesh->mBones[boneIndex]->mOffsetMatrix);
            boneInfoMap[boneName] = boneInfo;
            boneId = m_BoneCounter;
            m_BoneCounter++;
#if defined(DEBUG) || defined(ENABLE_LOG)
            std::cout << std::format("[Model: {}] Found bone: {} ({})\n", modelPath, boneName, m_BoneCounter);
#endif
        }
        else
            boneId = boneInfoMap[boneName].id;

        assert(boneId != -1);

        const aiVertexWeight *weights = mesh->mBones[boneIndex]->mWeights;
        const unsigned int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (unsigned int weightIndex = 0; weightIndex < numWeights; weightIndex++)
        {
            const unsigned int vertexId = weights[weightIndex].mVertexId;
            const float weight = weights[weightIndex].mWeight;
            assert(vertexId <= vertices.size());
            SetVertexBoneData(vertices[vertexId], boneId, weight);
        }
    }
}

void Model::SetVertexBoneData(Vertex &vertex, const int boneId, const float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.boneIds[i] < 0)
        {
            vertex.weights[i] = weight;
            vertex.boneIds[i] = boneId;
            break;
        }
    }
}
