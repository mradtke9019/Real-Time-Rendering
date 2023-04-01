#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h";
#include "IShader.h";
#include "Log.h"

class Shader : public IShader
{
private:
    std::string shaderName;
    std::string VertexShaderText;
    std::string FragmentShaderText;
    GLuint ShaderProgramID;
    
    /*glm::vec3 LightPosition;
    glm::vec3 LightColor;*/
    GLuint textureID;
    
    //https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
    std::string NameFromPath(std::string path)
    {
        std::string::size_type const p(path.find_last_of('.'));
        std::string file_without_extension = path.substr(0, p);
        return file_without_extension;
    }

    std::string ReadFile(const char* path)
    {
        std::ifstream stream(path);
        std::string file((std::istreambuf_iterator<char>(stream)),
            std::istreambuf_iterator<char>());
        return file;
    }

    void AddShader(const char* pShaderText, GLenum ShaderType)
    {
        // create a shader object
        GLuint ShaderObj = glCreateShader(ShaderType);

        if (ShaderObj == 0) {
            fprintf(stderr, "Error creating shader type %d\n", ShaderType);
            exit(0);
        }
        // Bind the source code to the shader, this happens before compilation
        glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderText, NULL);
        // compile the shader and check for errors
        glCompileShader(ShaderObj);
        GLint success;
        // check for shader related errors using glGetShaderiv
        glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar InfoLog[1024];
            glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
            fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
            exit(1);
        }
        // Attach the compiled shader object to the program object
        glAttachShader(ShaderProgramID, ShaderObj);
    }

    GLuint CompileShaders(std::string VertexShaderText, std::string FragmentShaderText)
    {
        //Start the process of setting up our shaders by creating a program ID
        //Note: we will link all the shaders together into this ID
        ShaderProgramID = glCreateProgram();
        if (ShaderProgramID == 0) {
            fprintf(stderr, "Error creating shader program\n");
            exit(1);
        }

        // Create two shader objects, one for the vertex, and one for the fragment shader
        AddShader(VertexShaderText.c_str(), GL_VERTEX_SHADER);
        AddShader(FragmentShaderText.c_str(), GL_FRAGMENT_SHADER);

        GLint Success = 0;
        GLchar ErrorLog[1024] = { 0 };


        // After compiling all shader objects and attaching them to the program, we can finally link it
        glLinkProgram(ShaderProgramID);
        // check for program related errors using glGetProgramiv
        glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &Success);
        if (Success == 0) {
            glGetProgramInfoLog(ShaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
            fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
            exit(1);
        }

        // program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
        glValidateProgram(ShaderProgramID);
        // check for program related errors using glGetProgramiv
        glGetProgramiv(ShaderProgramID, GL_VALIDATE_STATUS, &Success);
        if (!Success) {
            glGetProgramInfoLog(ShaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
            fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
            exit(1);
        }
        // Finally, use the linked shader program
        // Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
        //glUseProgram(ShaderProgramID);
        return ShaderProgramID;
    }

    void SetCubemap()
    {
        std::vector<std::string> textures_faces
        {
            "right.jpg",
            "left.jpg",
            "top.jpg",
            "bottom.jpg",
            "front.jpg",
            "back.jpg"
        };
        // parameters GLsizei n, GLuint* textures
        //n: the number of texture objects to be generated
        // textures: specifies an array in which the generated texture DIs are stored
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < textures_faces.size(); i++)
        {
            unsigned char* data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                Log::WriteLog("Cubemap tex failed to load at path: " + textures_faces[i], Error);
                //std::cout << "Cubemap tex failed to load at path: " << textures_faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

public:
    Shader() {};

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

    std::string GetShaderName();

    GLuint GetAttribLocation(std::string shaderAttribute);

    std::string GetVertexShaderText();

    std::string GetFragmentShaderText();

    GLuint GetShaderProgramID();

    void Use();

    GLuint GetUniformLocation(const char* uniform);

    void SetUniform1f(const char* uniform, float value);

    void SetUniform1i(const char* uniform, int value);

    GLuint GetUniformMatrix4fv(const char* mat);

    void SetUniformMatrix44fv(const char* mat, glm::mat4x4* matrix);

    void SetUniformMatrix4fv(const char* mat, glm::mat4* matrix);

    GLuint GetUniformVec3(const char* vec);

    void SetUniformVec3(const char* vec, glm::vec3 vector);
    void SetUniformFloatArray(const char* name, const float arr[], int count);
    void LoadCubemap(std::string textureUniform) {
        SetUniform1i(textureUniform.c_str(), 0);
        SetCubemap();
    }

};