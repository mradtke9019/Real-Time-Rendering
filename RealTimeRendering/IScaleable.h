
#pragma once
#include "glm/gtx/quaternion.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <exception>
#include "Log.h"


/// <summary>
/// Interface representing a rotatable object. Uses radians for its angle representations.
/// </summary>
class IScaleable 
{
private:
	float scaleX;
	float scaleY;
	float scaleZ;

	glm::mat4 ScaleMatrix;


public:
	IScaleable()
	{
		scaleX = 1.0f;
		scaleY = 1.0f;
		scaleZ = 1.0f;
		UpdateScaleMatrix();
	}
	void UpdateScaleMatrix()
	{
		ScaleMatrix = glm::scale(glm::mat4(1), glm::vec3(scaleX, scaleY, scaleZ));
	}

	float* GetScaleX()
	{
		return &scaleX;
	}
	float* GetScaleY()
	{
		return &scaleY;
	}
	float* GetScaleZ()
	{
		return &scaleZ;
	}

	glm::mat4 GetScaleMatrix()
	{
		return ScaleMatrix;
	}

};