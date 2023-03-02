#include "Bone.h"

Bone::Bone(Shader* s)
{
	LocalPosition = glm::vec3(0, 0, 0);
	BoneModel = Model("./Models/Cylinder/cylinder.obj", LocalPosition,s);
	parent = nullptr;
	children = std::vector<Bone*>();
}


Bone::Bone(Shader* s, Bone* parent)
{
	LocalPosition = glm::vec3(0, 0, 0);
	BoneModel = Model("./Models/Cylinder/cylinder.obj", LocalPosition, s);
	this->parent = parent;
	children = std::vector<Bone*>();
}

Bone::Bone(Shader* s, Bone* parent, glm::vec3 pos)
{
	LocalPosition = pos;
	BoneModel = Model("./Models/Cylinder/cylinder.obj", LocalPosition, s);
	this->parent = parent;
	children = std::vector<Bone*>();
}

/// <summary>
/// Uses the current bone as the root and attempts to move itself as well as its children so that all of its children will try to move towards the target point
/// </summary>
/// <param name="target"></param>
/// <returns></returns>
void Bone::RotateTowardsPosition(glm::vec3 target)
{
	const float arrivalDistance = 0.2f;

	// Set an epsilon value to prevent division by small numbers.
	const float epsilon = 0.0001;

	// Set max arc length a bone can move the end effector an be considered no motion
	// so that we can detect a failure state.
	const float trivialArcLength = 0.00001;

	Bone* root = this;
	std::vector<Bone*> bones = std::vector<Bone*>();
	bones.push_back(root);
	int numBones = 1;


	
	// Just do depth first layer of bones
	Bone* joint = this->GetFirstChild();

	while (joint != nullptr)
	{
		numBones++;
		bones.push_back(joint);
		joint = joint->GetFirstChild();
	}

	Bone* leafBone = bones[numBones - 1];
	// Tip of our point e
	glm::vec3 end = leafBone->GetGlobalTipPosition();

	// Stop if we hvae reached our threshold
	if (glm::length(end - target) < arrivalDistance)
	{
		return;
	}


	// Iterate through each bone and move them slightly towards the target
	for (int i = numBones - 1; i >= 0; --i)
	{
		// Imagine 3 different planes that we need to get to at the same time for each xy, xz, and yz plane
		// Represent the current bone as joint j
		joint = bones[i];
		end = leafBone->GetGlobalTipPosition();

		glm::vec3 j = joint->GetGlobalRootPosition();
		glm::vec3 e = end;
		glm::vec3 t = target;

		// Get the current bones difference from the leaf position
		glm::vec3 ej = e - j;
		ej = glm::normalize(ej);


		glm::vec3 tj = t - j;
		tj = glm::normalize(tj);

		float cosRotAng;

		cosRotAng = glm::dot(ej, tj);// Length of vectors should be 1 at this point, so do not need to divide by their magnitude, which is 1 anyway

		double rotAng = glm::acos(glm::max(-1.0f, glm::min(1.0f, cosRotAng)));
		rotAng = IRotatable::SimplifyAngle(rotAng);

		//Find sign of angle using cross product
		glm::vec3 cross = glm::cross(ej, tj);// (curToEndX * curToTargetY - curToEndY * curToTargetX) / endTargetMag;

		joint->SetAxis(cross);
		joint->IncrementAxisAngle(rotAng);
		joint->UseAxisAngle(true);

		glm::vec3 newLeafPosition = leafBone->GetGlobalTipPosition();

		float newDistance = glm::length(newLeafPosition - target);

		// Stop if we are already there
		if (newDistance < arrivalDistance)
		{
			return;
		}

	}

}