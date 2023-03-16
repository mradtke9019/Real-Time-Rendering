
#include <glm/gtc/matrix_transform.hpp>

#pragma once

class ITranslatable {
private:
	glm::vec3 Position;

public:
	ITranslatable()
	{
		Position = glm::vec3(0, 0, 0);
	}

	glm::mat4 GetTranslationMatrix()
	{
		return glm::translate(glm::mat4(1), Position);
	}

	glm::vec3 GetPosition()
	{
		return Position;
	}

	void SetPosition(glm::vec3 p)
	{
		Position = p;
	}
};