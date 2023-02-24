#include "glm/gtx/quaternion.hpp"


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