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
#include "Log.h"

class IShader
{
private:


public:

    virtual std::string GetShaderName() = 0;

    virtual void DebugOn() = 0;

    virtual void DebugOff() = 0;

    virtual GLuint GetAttribLocation(std::string shaderAttribute) = 0;

    virtual std::string GetVertexShaderText() = 0;

    virtual std::string GetFragmentShaderText() = 0;

    virtual GLuint GetShaderProgramID() = 0;

    virtual void Use() = 0;

    virtual GLuint GetUniformLocation(const char* uniform) = 0;

    virtual void SetUniform1f(const char* uniform, float value) = 0;

    virtual void SetUniform1i(const char* uniform, int value) = 0;

    virtual GLuint GetUniformMatrix4fv(const char* mat) = 0;

    virtual void SetUniformMatrix44fv(const char* mat, glm::mat4x4* matrix) = 0;

    virtual void SetUniformMatrix4fv(const char* mat, glm::mat4* matrix) = 0;

    virtual GLuint GetUniformVec3(const char* vec) = 0;

    virtual void SetUniformVec3(const char* vec, glm::vec3 vector) = 0;

    virtual void SetUniformFloatArray(const char* name, const float arr[], int count) = 0;

};