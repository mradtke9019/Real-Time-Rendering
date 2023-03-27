#include "Mesh.h"
// Inspiration for Mesh Class and cincepts here
// https://learnopengl.com/Model-Loading/Mesh


void Mesh::SetShader(IShader* shader)
{
    this->shader = shader;
}

IShader* Mesh::GetShader()
{
    return shader;
}


void Mesh::Draw(glm::mat4* ModelTransform)
{
    shader->Use();
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader->GetShaderProgramID(), (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // Translate mesh back to origin, rotate, then re apply the translation
    glm::mat4 t = (*ModelTransform) * glm::translate(glm::mat4(1), MeshOrigin) * MeshTransform * glm::translate(glm::mat4(1), -MeshOrigin);

    shader->SetUniformMatrix4fv("model", &t);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

/// <summary>
/// Draw the mesh with the specified textures
/// </summary>
/// <param name="ModelTransform"></param>
/// <param name="textures"></param>
void Mesh::Draw(glm::mat4* ModelTransform, std::vector<Texture> textures)
{
    shader->Use();
    // bind appropriate textures
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        string name = textures[i].name;
        // now set the sampler to the correct texture unit
        int samplerId = glGetUniformLocation(shader->GetShaderProgramID(), (name).c_str());
        if (samplerId < 0)
        {
            continue;
        }

        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding

        glUniform1i(samplerId, i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // Translate mesh back to origin, rotate, then re apply the translation
    glm::mat4 t = (*ModelTransform) * glm::translate(glm::mat4(1), MeshOrigin) * MeshTransform * glm::translate(glm::mat4(1), -MeshOrigin);

    shader->SetUniformMatrix4fv("model", &t);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}