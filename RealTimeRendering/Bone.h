
#include "Model.h"

#pragma once
class Bone : public IRotatable
{
private:
	Bone* parent;
	Bone* child;
	Model BoneModel;
	glm::mat4 Transform;
	const float Length = 1;
	const glm::vec3 Axis = glm::vec3(0,1,0);

public:

	Bone(Shader * s);
	Bone(Shader* s, Bone* parent);

	// Retrieves the global transform that the current bone would take place in. Recursive and will use its parents transform if it exists.
	glm::mat4 GetGlobalTransform()
	{
		if (parent != nullptr)
			return parent->GetGlobalTransform() * Transform;
		else
			return Transform;
	}

	void AddBone(Bone* bone)
	{
		child = bone;
		child->SetParent(this);
		child->Transform = glm::translate(glm::mat4(1), Axis);
	}

	void SetParent(Bone* b)
	{
		this->parent = b;
	}

	void Draw()
	{
		BoneModel.Draw();
	}
};

