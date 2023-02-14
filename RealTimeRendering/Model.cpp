#include "Model.h"

Model::Model(std::string path, glm::vec3 Position, Shader* Shader)
	: ModelTransform(glm::mat4(1.0f))
{
	shader = Shader;
	ObjectColor = glm::vec3(1.0, 0.0, 0.0);
	ModelTransform = glm::translate(glm::mat4(1.0f), Position);
	this->Position = Position;
	loadModel(path);
}

Model::Model(std::string path, glm::vec3 Position, Shader* Shader, glm::vec3 color)
	: ModelTransform(glm::mat4(1.0f))
{
	shader = Shader;
	ObjectColor = color;
	this->Position = Position;
	ModelTransform = glm::translate(glm::mat4(1.0f), Position);
	loadModel(path);
}

void Model::Draw()
{
	shader->Use();
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

glm::mat4 Model::GetModelTransform()
{
	return ModelTransform;
}

void Model::SetColor(glm::vec3 color)
{
	this->ObjectColor = color;
}

glm::vec3 Model::GetColor()
{
	return this->ObjectColor;
}


glm::vec3 Model::GetPosition()
{
	return this->Position;
}

void Model::SetPosition(glm::vec3 pos)
{
	this->Position = pos;
	UpdateModelTransform();
}


void Model::RotateX(float angle)
{
	IRotatable::RotateX(angle);
	UpdateModelTransform();
}

void Model::RotateY(float angle)
{
	IRotatable::RotateY(angle);
	UpdateModelTransform();
}

void Model::RotateZ(float angle)
{
	IRotatable::RotateZ(angle);
	UpdateModelTransform();
}

void Model::RotateMeshX(int index, float x)
{
	meshes.at(index).RotateX(x);
}
void Model::RotateMeshY(int index, float x)
{

	meshes.at(index).RotateY(x);
}
void Model::RotateMeshZ(int index, float x)
{
	meshes.at(index).RotateZ(x);
}