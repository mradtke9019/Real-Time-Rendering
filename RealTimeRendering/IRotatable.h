
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
		if (!useQuaternions) 
		{
			result = 
				glm::rotate(result, glm::radians(GetRotateX()), glm::vec3(1.0f, 0.0f, 0.0f)) * 
				glm::rotate(result, glm::radians(GetRotateY()), glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(result, glm::radians(GetRotateZ()), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		return result;
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