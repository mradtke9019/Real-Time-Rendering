#include "FixedCamera.h"

FixedCamera::FixedCamera(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
	this->SetPosition(position);
	Target = target;
	Up = up;

	View = glm::lookAt(this->GetPosition(), Target, Up);
};

glm::mat4* FixedCamera::GetViewTransform()
{
	View = glm::lookAt(this->GetPosition(), Target, Up);
	return &View;
};

void FixedCamera::SetTarget(glm::vec3 target)
{
	Target = target;
};

glm::vec3 FixedCamera::GetTarget()
{
	return Target;
};
