#include "glm/gtx/quaternion.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#pragma once
class IRotatable {
private:

	float rotateX;
	float rotateY;
	float rotateZ;

	bool useQuaternions;


public:
	IRotatable()
	{
		rotateX = 0;
		rotateY = 0;
		rotateZ = 0;

		useQuaternions = false;
	}


	//Return angle clamped between 0 and 360 or 0 and 2 pi
	static float SimplifyAngle(float angle, bool isDegrees = false)
	{
		if (!isDegrees)
		{
			angle = fmod(angle, (2.0 * M_PI));
			if (angle < -M_PI)
				angle += (2.0 * M_PI);
			else if (angle > M_PI)
				angle -= (2.0 * M_PI);
			return angle;
		}
		else
		{
			angle = fmod(angle, 360.0f);
			if (angle < -M_PI)
				angle += 360.0f;
			else if (angle > M_PI)
				angle -= 360.0f;
			return angle;
		}
	}

	glm::mat4 GetRotationMatrix()
	{
		glm::mat4 result = glm::mat4(1);
		if (useQuaternions) 
		{

			// Reference: https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions
			glm::quat x = glm::angleAxis(glm::radians(rotateX), glm::vec3(1, 0, 0));
			glm::quat y = glm::angleAxis(glm::radians(rotateY), glm::vec3(0, 1, 0));
			glm::quat z = glm::angleAxis(glm::radians(rotateZ), glm::vec3(0, 0, 1));

			// Order matters?
			glm::quat quaternion = y * z * x;
			result = glm::toMat4(quaternion);
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
	

	bool UsingQuaternions()
	{
		return useQuaternions;
	}

	void SetQuaternionPolicy(bool use)
	{
		useQuaternions = use;
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


	
};