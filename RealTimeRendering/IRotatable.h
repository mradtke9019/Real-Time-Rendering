#include "glm/gtx/quaternion.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#pragma once
class IRotatable {
private:

	float rotateX;
	float rotateY;
	float rotateZ;
	glm::vec3 Axis;
	float angle;

	bool useAxisAngle;


public:
	IRotatable()
	{
		rotateX = 0;
		rotateY = 0;
		rotateZ = 0;

		useAxisAngle = false;
		Axis = glm::vec3(0,0,0);
		angle = 0.0f;
	}


	//Return angle clamped between 0 and 2 pi
	// https://www.ryanjuckett.com/cyclic-coordinate-descent-in-2d/
	static float SimplifyAngle(float angle)
	{
		angle = fmod(angle, (2.0 * M_PI));
		if (angle < -M_PI)
			angle += (2.0 * M_PI);
		else if (angle > M_PI)
			angle -= (2.0 * M_PI);
		return angle;
	}

	glm::mat4 GetRotationMatrix()
	{
		glm::mat4 result = glm::mat4(1);
		if (useAxisAngle) 
		{
			if (glm::length(Axis) <= 0 || Axis == glm::vec3(0, 0, 0))
				return glm::mat4(1);
			result = glm::toMat4(glm::angleAxis(angle, Axis));
			glm::vec3 euler =glm::eulerAngles(glm::angleAxis(angle, Axis));
			// Update our euler angles representation

			rotateX = glm::degrees(euler.x);
			rotateY = glm::degrees(euler.y);
			rotateZ = glm::degrees(euler.z);
		}
		else
		{
			result =
				glm::rotate(result, glm::radians(*GetRotateY()), glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(result, glm::radians(*GetRotateZ()), glm::vec3(0.0f, 0.0f, 1.0f)) *
				glm::rotate(result, glm::radians(*GetRotateX()), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		return result;
	}
	

	bool UsingAxisAngle()
	{
		return useAxisAngle;
	}

	void UseAxisAngle(bool use)
	{
		useAxisAngle = use;
	}

	float* GetRotateX()
	{
		return &rotateX;
	}

	float* GetRotateY()
	{
		return &rotateY;
	}
	float* GetRotateZ()
	{
		return &rotateZ;
	}

	void SetRotateX(float x)
	{
		rotateX = x;
	}

	void SetRotateY(float y)
	{
		rotateY = y;
	}

	void SetRotateZ(float z)
	{
		rotateZ = z;
	}

	void RotateX(float x)
	{
		rotateX += x;
	}

	void RotateY(float y)
	{
		rotateY += y;
	}

	void RotateZ(float z)
	{
		rotateZ += z;
	}

	void SetAxis(glm::vec3 a)
	{
		if (a == glm::vec3(0, 0, 0))
			Axis = -Axis;
		else
			Axis = glm::normalize(a);
	}

	void SetAxisAngle(float a)
	{
		angle = a;
	}

	void IncrementAxisAngle(float a)
	{
		angle += a;
	}

	float GetAxisAngle()
	{
		return angle;
	}
	
};