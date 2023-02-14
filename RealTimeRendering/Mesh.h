#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shader.h"
#include "Log.h"
#include "IRotatable.h"


// Ideas used here for model loading via assimp
// https://learnopengl.com/Model-Loading/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

#pragma once
class Mesh: IRotatable {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;

    //Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Shader* shader);
    void SetShader(Shader* Shader);
    Shader* GetShader();
    void Draw(glm::mat4* ModelTransform = nullptr);

    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Shader* s)
        : MeshOrigin(glm::vec3(0,0,0)), ModelOrigin(glm::vec3(0,0,0))
    {
        MeshTransform = glm::mat4(1);
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        this->shader = s;
        if (this->vertices.size() <= 0 || this->indices.size() <= 0) {
            Log::WriteLog("Error on mesh. No vertices or indices.", Error);
        }
        else {
            // now that we have all the required data, set the vertex buffers and its attribute pointers.
            setupMesh();
        }
    }

    void RotateX(float x);
    void RotateY(float x);
    void RotateZ(float x);

    void SetOrigin(glm::vec3 mesh, glm::vec3 model);

private:
    // render data 
    glm::mat4 MeshTransform;
    unsigned int VBO, EBO;
    Shader* shader;
    glm::vec3 ModelOrigin;
    glm::vec3 MeshOrigin;

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        unsigned int vPosition = shader->GetAttribLocation("vPosition");
        glEnableVertexAttribArray(vPosition);
        glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        
        // vertex normals
        int vNormal = shader->GetAttribLocation("vNormal");
        // For some reason cant find vNormal, so just default to 1 if cant be found
        if (vNormal < 0) {
            Log::WriteLog("Cant find vNormal", Error);
            vNormal = 1;
        }

        glEnableVertexAttribArray(vNormal);
        glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        
        // vertex texture coords
        unsigned int vTexture = shader->GetAttribLocation("vTexture");
        glEnableVertexAttribArray(vTexture);
        glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        
        // vertex tangent
        unsigned int vTangent = shader->GetAttribLocation("vTangent");
        glEnableVertexAttribArray(vTangent);
        glVertexAttribPointer(vTangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        
        // vertex bitangent
        unsigned int vBitangent = shader->GetAttribLocation("vBitangent");
        glEnableVertexAttribArray(vBitangent);
        glVertexAttribPointer(vBitangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    }

    void UpdateMeshTransform()
    {
        MeshTransform = IRotatable::GetRotationMatrix();
    }
};
