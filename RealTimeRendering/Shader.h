#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
    std::string VertexShaderText;
    std::string FragmentShaderText;
    GLuint ShaderProgramID;
    bool debugShader;
    glm::vec3 LightPosition;
    glm::vec3 LightDirection;
    glm::vec3 LightColor;

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

public:
    Shader() {};
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

    void DebugOn();

    void DebugOff();

    GLuint GetAttribLocation(std::string shaderAttribute);

    std::string GetVertexShaderText();

    std::string GetFragmentShaderText();

    GLuint GetShaderProgramID();

    void Use();

    GLuint GetUniformLocation(const char* uniform);

    void SetUniform1f(const char* uniform, float value);

    GLuint GetUniformMatrix4fv(const char* mat);

    void SetUniformMatrix44fv(const char* mat, glm::mat4x4* matrix);

    void SetUniformMatrix4fv(const char* mat, glm::mat4* matrix);

    GLuint GetUniformVec3(const char* vec);

    void SetUniformVec3(const char* vec, glm::vec3 vector);

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value);
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const;
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const;
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w);
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const;

};