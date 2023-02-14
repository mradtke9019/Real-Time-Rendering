#include "glm/gtx/quaternion.hpp"


#pragma once
class IRotatable {
private:
	glm::vec3 rotationAxis;
	float rotationAngle;

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

		rotationAxis = glm::vec3(0,0,1);
		rotationAngle = 0.0f;
	}

	glm::mat4 GetRotationMatrix()
	{
		glm::mat4 result = glm::mat4(1);
		if (useQuaternions) 
		{
			// Normalize our axis so no strange things happen with the quaternions
			rotationAxis = glm::normalize(rotationAxis);

			// Reference: https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions
			float x = rotationAxis.x * glm::sin(glm::radians(rotationAngle / 2));
			float y = rotationAxis.y * glm::sin(glm::radians(rotationAngle / 2));
			float z = rotationAxis.z * glm::sin(glm::radians(rotationAngle/ 2));
			float w = glm::cos(glm::radians(rotationAngle / 2));


			glm::quat quaternion = glm::quat(w,x,y,z);
			result = glm::toMat4(quaternion);
		}
		else
		{
			result =
				glm::rotate(result, glm::radians(GetRotateX()), glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::rotate(result, glm::radians(GetRotateY()), glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(result, glm::radians(GetRotateZ()), glm::vec3(0.0f, 0.0f, 1.0f));
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

	void SetRotationAxisX(float x)
	{
		rotationAxis.x = x;
	}
	void SetRotationAxisY(float x)
	{
		rotationAxis.y = x;
	}
	void SetRotationAxisZ(float x)
	{
		rotationAxis.z = x;
	}


	float GetRotationAxisX()
	{
		return rotationAxis.x;
	}
	float GetRotationAxisY()
	{
		return rotationAxis.y;
	}
	float GetRotationAxisZ()
	{
		return rotationAxis.z;
	}

	void IncreaseRotationAngle(float x)
	{
		rotationAngle += x;
	}

	float GetRotateX()
	{
		return rotateX;
	}

	float GetRotateY()
	{
		return rotateY;
	}
	float GetRotateZ()
	{
		return rotateZ;
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