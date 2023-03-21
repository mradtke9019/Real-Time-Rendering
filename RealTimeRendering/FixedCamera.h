#pragma once
#include "ICamera.h"

class FixedCamera : public ICamera
{
private:

	glm::vec3 Target;
	glm::vec3 Up;

	glm::mat4 View;

public:

	FixedCamera() {};
	FixedCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up);

	glm::mat4* GetViewTransform();

	glm::vec3 GetTarget();

	void SetTarget(glm::vec3);
};