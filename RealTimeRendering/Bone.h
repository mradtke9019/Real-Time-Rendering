
#include "Model.h"

#pragma once
enum CCDResult {Success,Processing,Failure};

class Bone : public IRotatable
{
private:
	Bone* parent;
	std::vector<Bone*> children;
	Model BoneModel;
	glm::vec3 LocalPosition;
	const float Length = 1;
	const glm::vec3 Axis = glm::vec3(0,1,0);
	const glm::vec3 Origin = glm::vec3(0, 0, 0);
	glm::vec3 Target;
	int Idx;
	bool MoveToTarget;


	glm::mat4 GetLocalTransform()
	{
		// Translate to origin based on where it is in the tree
		glm::vec3 t = glm::vec3(0, 0, 0);
		// Undo wherever we are in global coordinates
		if (parent != nullptr)
		{
			glm::vec3 t = parent->GetGlobalRootPosition();// *glm::vec4(Origin, 1);
		}
		return glm::translate(glm::mat4(1), LocalPosition) * glm::translate(glm::mat4(1), t) *  GetRotationMatrix()  * glm::translate(glm::mat4(1), -t);
		//return glm::translate(glm::mat4(1), Axis * Length) * glm::translate(glm::mat4(1), t) * GetRotationMatrix() * glm::translate(glm::mat4(1), -t);
	}

public:
	Bone() {};
	Bone(Shader* s);
	Bone(Shader* s, Bone* parent);
	Bone(Shader* s, Bone* parent, glm::vec3 pos);

	/// <summary>
	/// Sets the target that this bone and its children should move towards.
	/// </summary>
	/// <param name="t"></param>
	void SetTarget(glm::vec3 t);

	glm::vec3* GetTarget();

	bool* MovingToTarget()
	{
		return &MoveToTarget;
	}

	int* GetIdx()
	{
		return &Idx;
	}

	/// <summary>
	/// Returns all bones in the current sub tree
	/// </summary>
	/// <returns></returns>
	std::vector<Bone*> GetAllBones()
	{
		std::vector<Bone*> result = std::vector<Bone*>();

		for (auto x : this->GetChildren())
		{
			result.push_back(x);
			for (auto y : x->GetAllBones())
			{
				result.push_back(y);
			}
		}

		return result;
	}


	void IterateTowardsTarget();

	// Retrieves the global transform that the current bone would take place in. Recursive and will use its parents transform if it exists.
	glm::mat4 GetGlobalTransform()
	{
		if (parent != nullptr)
			return  parent->GetGlobalTransform() * GetLocalTransform();
		else
			return GetLocalTransform();
	}

	Bone* GetRoot()
	{
		Bone* curr = this;

		while (curr->parent != nullptr)
		{
			curr = curr->parent;
		}
		return curr;
	}

	glm::vec3 GetGlobalRootPosition()
	{
		return glm::vec3(GetGlobalTransform() * glm::vec4(GetRoot()->LocalPosition, 1.0f));
	}

	// Get the tip position of the cylinder bone based off the root position
	glm::vec3 GetGlobalTipPosition()
	{
		// I know the root position code works, so use child position as root if it exists
		if (children.size() > 0)
		{
			return children.at(0)->GetGlobalRootPosition();
		}

		// To find tip, create bone child and get its root position, then remove bone
		int idx = children.size();
		AddBone(new Bone());

		glm::vec3 tip = children.at(idx)->GetGlobalRootPosition();
		Bone* t = RemoveBone(idx);
		delete t;
		t = nullptr;

		return tip;
	}

	void AddBone(Bone* bone)
	{
		children.push_back(bone);
		bone->SetParent(this);
		bone->LocalPosition = Axis * Length;
	}

	// Remove bone from children list
	Bone* RemoveBone(int idx)
	{
		Bone* bone = children.at(idx);
		children.erase(children.begin() + idx);
		return bone;
	}

	int GetDepth()
	{
		int depth = 0;
		Bone* curr = this->parent;
		while (curr != nullptr)
		{
			curr = curr->parent;
			depth++;
		}
		return depth;
	}

	static int GetTreeHeight(Bone* root, int currHeight = 1)
	{
		int max = -1;
		std::vector<int> depths = std::vector<int>();
		for (int i = 0; i < root->children.size(); i++)
		{
			depths.push_back(GetTreeHeight(root->children.at(i), currHeight + 1));
		}
		for (int j = 0; j < depths.size(); j++)
			if (depths[j] > max)
				max = depths[j];
		if (currHeight > max)
			max = currHeight;
		return max;
	}

	std::vector<Bone*> GetAllDescendants()
	{
		std::vector<Bone*> result = std::vector<Bone*>();
		std::vector<Bone*> children = GetChildren();
		result.push_back(this);
		for (int i = 0; i < children.size(); i++)
		{
			std::vector<Bone*> other = children.at(i)->GetAllDescendants();
			result.push_back(children[i]);
			for (int j = 0; j < other.size(); j++)
			{
				result.push_back(other[i]);
			}
		}

		return result;
	}

	std::vector<Bone*> GetChildren()
	{
		return this->children;
	}

	Bone* GetFirstChild()
	{
		Bone* r = nullptr;

		if (this->children.size() > 0)
		{
			r = children.at(0);
		}

		return r;
	}

	void SetParent(Bone* b)
	{
		this->parent = b;
	}

	void Draw(bool drawChildren = true)
	{
		if (MoveToTarget)
		{
			IterateTowardsTarget();
		}
		else {
			UseAxisAngle(false);
		}
		BoneModel.Draw(GetGlobalTransform());
		if (drawChildren)
		{
			for (int i = 0; i < children.size(); i++)
			{
				children.at(i)->Draw(drawChildren);
			}
		}
	}
};