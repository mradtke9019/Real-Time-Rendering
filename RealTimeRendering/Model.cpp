#include "Model.h"

Model::Model(std::string path, glm::vec3 Position, Shader* Shader)
	: ITransformable(), mipmap(GL_LINEAR_MIPMAP_LINEAR)
{
	shader = Shader;
	ObjectColor = glm::vec3(1.0, 0.0, 0.0);
	this->SetPosition(Position);
	loadModel(path);
}

Model::Model(std::string path, glm::vec3 Position, Shader* Shader, glm::vec3 color)
	: ITransformable(), mipmap(GL_LINEAR_MIPMAP_LINEAR)
{
	shader = Shader;
	ObjectColor = color;
	this->SetPosition(Position);
	loadModel(path);
}

// Constructor to control the mip mapping generated
Model::Model(std::string path, glm::vec3 Position, Shader* Shader, GLint mipmapPolicy)
	: ITransformable(), mipmap(GL_LINEAR_MIPMAP_LINEAR)
{
	shader = Shader;
	ObjectColor = glm::vec3(1.0, 0.0, 0.0);
	this->SetPosition(Position);
	loadModel(path);
}

void Model::Draw()
{
	shader->Use();
	glm::mat4 ModelTransform = this->GetTransformation();
	shader->SetUniformMatrix4fv("model", &ModelTransform);
	shader->SetUniformVec3("ObjectColor", ObjectColor);
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i).Draw(&ModelTransform);
	}
}


Shader* Model::GetShader()
{
	return shader;
}

void Model::SetShader(Shader* Shader)
{
	shader = Shader;
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i).SetShader(shader);
	}
}

void Model::SetColor(glm::vec3 color)
{
	this->ObjectColor = color;
}

glm::vec3 Model::GetColor()
{
	return this->ObjectColor;
}
