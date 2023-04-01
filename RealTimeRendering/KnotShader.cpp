#include "KnotShader.h"


KnotShader::KnotShader(const char* vertexShaderPath, const char* fragmentShaderPath) :
	Shader(vertexShaderPath, fragmentShaderPath)
{
	LoadDefaultWoodTextures();
}


void KnotShader::SetRMin(float rmin)
{
	this->rmin = rmin;
	this->SetUniform1f("rmin", rmin);
}

void KnotShader::SetRMax(float rmax)
{
	this->rmax = rmax;
	this->SetUniform1f("rmax", rmax);
}

void KnotShader::SetHMax(float hmax)
{
	this->hmax = hmax;
	this->SetUniform1f("hmax", hmax);
}

void KnotShader::SetKNum(float knum)
{
	this->knum = knum;
	this->SetUniform1f("rmin", rmin);
}

void KnotShader::Use()
{
	Shader::Use();

	// bind appropriate textures
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string name = textures[i].name;
		// now set the sampler to the correct texture unit
		int samplerId = glGetUniformLocation(this->GetShaderProgramID(), (name).c_str());
		if (samplerId < 0)
		{
			Log::WriteLog("Texture not found in shader: " + name, Warning);
			continue;
		}

		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding

		glUniform1i(samplerId, i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
}