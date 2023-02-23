
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
	const glm::vec3 Axis = glm::vec3(0,2,0);
	const glm::vec3 Origin = glm::vec3(0, 0, 0);

	glm::mat4 GetLocalTransform()
	{
		// Translate to origin based on where it is in the tree
		glm::vec3 t = glm::vec3(0, 0, 0);
		// Undo wherever we are in global coordinates
		if (parent != nullptr)
		{
			glm::vec3 t = glm::inverse(parent->GetGlobalTransform()) * glm::vec4(Origin, 1);
		}
		return glm::translate(glm::mat4(1), Axis) * glm::translate(glm::mat4(1), t) *  GetRotationMatrix()  * glm::translate(glm::mat4(1), -t);
	}

public:

	Bone(Shader * s);
	Bone(Shader* s, Bone* parent);

	// Retrieves the global transform that the current bone would take place in. Recursive and will use its parents transform if it exists.
	glm::mat4 GetGlobalTransform()
	{
		if (parent != nullptr)
			return parent->GetGlobalTransform() * GetLocalTransform();
		else
			return GetLocalTransform();
	}

	void AddBone(Bone* bone)
	{
		child = bone;
		child->SetParent(this);
		child->Transform = glm::translate(glm::mat4(1), Axis);
	}

	Bone* GetChild()
	{
		return this->child;
	}

	void SetParent(Bone* b)
	{
		this->parent = b;
	}

	void Draw(bool drawChildren = true)
	{
		BoneModel.Draw(GetGlobalTransform());
		if (drawChildren && this->child != nullptr)
		{
			child->Draw(drawChildren);
		}
	}
};