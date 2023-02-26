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
CCDResult Bone::RotateTowardsPosition(glm::vec3 target)
{
	const float arrivalDistance = 0.25f;

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
	Bone* curr = this->GetFirstChild();

	while (curr != nullptr)
	{
		numBones++;
		bones.push_back(curr);
		curr = curr->GetFirstChild();
	}

	Bone* leafBone = bones[numBones - 1];
	glm::vec3 leafBonePosition = leafBone->GetGlobalTipPosition();
	bool modified = false;

	// Iterate through each bone and move them slightly towards the target
	for (int i = numBones - 2; i >= 0; i--)
	{
		curr = bones[i];
		glm::vec3 currPos = curr->GetGlobalTipPosition();

		// Get the current bones difference from the leaf position
		glm::vec3 currLeafDiff = leafBonePosition - currPos;
		float currLeafDiffMagnitude = glm::length(currLeafDiff);


		glm::vec3 currTargetDiff = target - currPos;
		float currTargetDiffMagnitude = glm::length(currTargetDiff);

		float cosRotAng;
		float sinRotAng;
		float leafTargetMagntitude = (currLeafDiffMagnitude * currTargetDiffMagnitude);

		if (leafTargetMagntitude <= epsilon)
		{
			cosRotAng = 1;
			sinRotAng = 0;
		}
		else
		{
			cosRotAng = glm::dot(currLeafDiff, currTargetDiff) / leafTargetMagntitude;  //(curToEndX * curToTargetX + curToEndY * curToTargetY) / endTargetMag;
			sinRotAng = 0;// glm::cross(currLeafDiff, currTargetDiff) / leafTargetMagntitude;// (curToEndX * curToTargetY - curToEndY * curToTargetX) / endTargetMag;
		}

		double rotAng = glm::acos(glm::max(-1.0f, glm::min(1.0f, cosRotAng)));
		rotAng = IRotatable::SimplifyAngle(rotAng, true);
		curr->SetRotateX(*curr->GetRotateX() + rotAng);

		glm::vec3 newLeafPosition = leafBone->GetGlobalTipPosition();

		float newDistance = glm::length(newLeafPosition - target);

		// We made it
		if (newDistance < arrivalDistance)
		{
			return Success;
		}

		if (!modified && glm::abs(rotAng) * currLeafDiffMagnitude > trivialArcLength)
		{
			modified = true;
		}
	}

	// We failed to find a valid solution during this iteration.
	if (modified)
		return Processing;
	else
		return Failure;
}