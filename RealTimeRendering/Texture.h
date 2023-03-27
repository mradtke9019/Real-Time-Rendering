#pragma once
#include <string>
#include "stb_image.h"
#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Log.h"

class Texture
{
private:
public:
    unsigned int id;
    std::string name;
    std::string type;
    std::string path;

    static Texture TextureFromFile(const char* path, const std::string& directory, const std::string& name, const std::string& type)
    {
        Texture tex = Texture();
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Source LearnOpenGL: https://learnopengl.com/Getting-started/Textures
            //GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the pixel sizeand uses nearest neighbor interpolation for texture sampling.
            //GL_LINEAR_MIPMAP_NEAREST : takes the nearest mipmap leveland samples that level using linear interpolation.
            //GL_NEAREST_MIPMAP_LINEAR : linearly interpolates between the two mipmaps that most closely match the size of a pixeland samples the interpolated level via nearest neighbor interpolation.
            //GL_LINEAR_MIPMAP_LINEAR : linearly interpolates between the two closest mipmaps and samples the interpolated level via linear interpolation.
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            Log::WriteLog("Texture failed to load at path: " + std::string(path), LogLevel::Error);
            stbi_image_free(data);
        }

        tex.id = textureID;
        tex.name = name;
        tex.type = type;
        tex.path = path;

        return tex;
    }

};

