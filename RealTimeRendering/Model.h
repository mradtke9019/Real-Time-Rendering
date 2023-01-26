#include "Shader.h"
#include "Mesh.h"
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma once
// Ideas used here for model loading via assimp
// https://learnopengl.com/Model-Loading/
class Model
{
private:
    Shader* shader;
    // model data
    std::vector<Mesh> meshes;
    std::string directory;
    glm::mat4 ModelTransform;
    glm::vec3 Position;
    glm::vec3 ObjectColor;

    void LoadModel(std::string path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);


        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            fprintf(stderr, "Assimp Error: '%s'\n", importer.GetErrorString());
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene);
    }

    void ProcessNode(aiNode* node, const aiScene* scene)
    {
        // process all the node's meshes (if any)
        for (int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // Extract vertex data from mesh
        for (int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex v;
            // Get Position
            v.Position.x = mesh->mVertices[i].x;
            v.Position.y = mesh->mVertices[i].y;
            v.Position.z = mesh->mVertices[i].z;

            v.Normal.x = mesh->mNormals[i].x;
            v.Normal.y = mesh->mNormals[i].y;
            v.Normal.z = mesh->mNormals[i].z;


            if (mesh->mTextureCoords[0])
            {
                v.TexCoords.x = mesh->mTextureCoords[0][i].x;
                v.TexCoords.y = mesh->mTextureCoords[0][i].y;
            }
            else
            {
                v.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(v);
        }

        // Get the vertex indices of the faces
        for (int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = LoadMaterialTextures(material,
                aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<Texture> specularMaps = LoadMaterialTextures(material,
                aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(vertices, indices, textures, shader);
    }
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        //for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        //{
        //    aiString str;
        //    mat->GetTexture(type, i, &str);
        //    Texture texture;
        //    texture.id = TextureFromFile(str.C_Str(), directory);
        //    texture.type = typeName;
        //    texture.path = str;
        //    textures.push_back(texture);
        //}
        return textures;
    }
public:
    Model() {};
    Model(std::string path, glm::vec3, Shader* Shader);
    Model(std::string path, glm::vec3 Position, Shader* Shader, glm::vec3 color);
    void Draw();
    void Draw(Shader* shader);
    Shader* GetShader();
    void SetShader(Shader*);
    void SetModelTransform(glm::mat4);
    glm::mat4 GetModelTransform();
    void SetColor(glm::vec3 color);
    glm::vec3 GetColor();
    glm::vec3 GetPosition();
};