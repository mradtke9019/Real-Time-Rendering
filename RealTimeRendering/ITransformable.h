#include "ITranslatable.h"
#include "IRotatable.h"

#pragma once

class ITransformable : public ITranslatable, public IRotatable {
private:

public:
	ITransformable()
		: ITranslatable(), IRotatable()
	{

	}

	glm::mat4 GetTransformation()
	{
		return GetTranslationMatrix() * GetRotationMatrix();
	}
};