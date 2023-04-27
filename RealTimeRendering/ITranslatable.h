
#include <glm/gtc/matrix_transform.hpp>

#pragma once

class ITranslatable {
private:
	glm::vec3 Position;
	glm::mat4 TranslationMatrix;



public:
	ITranslatable()
	{
		Position = glm::vec3(0, 0, 0);
		UpdateTranslationMatrix();
	}	
	
	void UpdateTranslationMatrix()
	{
		TranslationMatrix = glm::translate(glm::mat4(1), Position);
	}


	glm::mat4 GetTranslationMatrix()
	{
		return TranslationMatrix;
	}

	glm::vec3 GetPosition()
	{
		return Position;
	}


	glm::vec3* GetPositionAddress()
	{
		return &Position;
	}

	void SetPosition(glm::vec3 p)
	{
		Position = p;
		UpdateTranslationMatrix();
	}
};