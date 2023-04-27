#pragma once
#include "ITranslatable.h"
#include "IRotatable.h"
#include "IScaleable.h"


class ITransformable : public ITranslatable, public IRotatable, public IScaleable 
{
private:
	glm::mat4 transform;


public:
	ITransformable()
		: ITranslatable(), IRotatable(), IScaleable()
	{
		transform = GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
	}

	void Update()
	{
		ITranslatable::UpdateTranslationMatrix();
		IScaleable::UpdateScaleMatrix();
		transform = GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
	}

	glm::mat4 GetTransformation()
	{
		return transform;
	}
};