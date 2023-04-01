#include "Model.h"

Model::Model(std::string path, glm::vec3 Position, IShader* Shader)
	: ITransformable(), mipmap(GL_LINEAR_MIPMAP_LINEAR), shouldDraw(true)
{
	shader = Shader;
	ObjectColor = glm::vec3(1.0, 0.0, 0.0);
	this->SetPosition(Position);
	loadModel(path);
}

Model::Model(std::string path, glm::vec3 Position, IShader* Shader, glm::vec3 color)
	: ITransformable(), mipmap(GL_LINEAR_MIPMAP_LINEAR), shouldDraw(true)
{
	shader = Shader;
	ObjectColor = color;
	this->SetPosition(Position);
	loadModel(path);
}

// Constructor to control the mip mapping generated
Model::Model(std::string path, glm::vec3 Position, IShader* Shader, GLint mipmapPolicy)
	: ITransformable(), mipmap(GL_LINEAR_MIPMAP_LINEAR), shouldDraw(true)
{
	shader = Shader;
	ObjectColor = glm::vec3(1.0, 0.0, 0.0);
	this->SetPosition(Position);
	loadModel(path);
}


bool* Model::ShouldDraw()
{
	return &shouldDraw;
}

void Model::Draw()
{
	if (!shouldDraw)
	{
		return;
	}
	shader->Use();
	glm::mat4 ModelTransform = this->GetTransformation();
	shader->SetUniformMatrix4fv("model", &ModelTransform);
	shader->SetUniformVec3("ObjectColor", ObjectColor);
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i).Draw(&ModelTransform);
	}
}

void Model::Draw(std::vector<Texture> textures)
{
	if (!shouldDraw)
	{
		return;
	}
	shader->Use();
	glm::mat4 ModelTransform = this->GetTransformation();
	shader->SetUniformMatrix4fv("model", &ModelTransform);
	shader->SetUniformVec3("ObjectColor", ObjectColor);
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i).Draw(&ModelTransform, textures);
	}
}


IShader* Model::GetShader()
{
	return shader;
}

void Model::SetShader(IShader* Shader)
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
