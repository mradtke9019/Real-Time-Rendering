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
	const float arrivalDistance = 0.1f;

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

	bool modified = false;

	// Iterate through each bone and move them slightly towards the target
	for (int i = numBones - 1; i >= 0; --i)
	{
		for (int itr = 0; itr < 3; itr++)
		{
			// Imagine 3 different planes that we need to get to at the same time for each xy, xz, and yz plane
			// Represent the current bone as joint j
			joint = bones[i];
			glm::vec3 j3D = joint->GetGlobalRootPosition();
			glm::vec2 j;
			glm::vec2 e;
			glm::vec2 t;

			switch (itr)
			{
			case 0:
				j = glm::vec2(j3D.x, j3D.y);
				e = glm::vec2(end.x, end.y);
				t = glm::vec2(target.x, target.y);
				break;
			case 1:
				j = glm::vec2(j3D.x, j3D.z);
				e = glm::vec2(end.x, end.z);
				t = glm::vec2(target.x, target.z);
				break;
			case 2:
				j = glm::vec2(j3D.y, j3D.z);
				e = glm::vec2(end.y, end.z);
				t = glm::vec2(target.y, target.z);
				break;
			}

			//glm::vec3 j = joint->GetGlobalTipPosition();

			// Get the current bones difference from the leaf position
			glm::vec2 ej = e - j;
			float ejMag = glm::length(ej);


			glm::vec2 tj = t - j;
			float tjMag = glm::length(tj);

			float cosRotAng;
			float sinRotAng;
			float ejMag_tjMag = (ejMag * tjMag);


			cosRotAng = glm::dot(ej, tj) / ejMag_tjMag;

			double rotAng = glm::acos(glm::max(-1.0f, glm::min(1.0f, cosRotAng)));
			rotAng = IRotatable::SimplifyAngle(rotAng, true);

			//Find sign of angle using cross product
			glm::vec3 signedCross = glm::cross(glm::vec3(ej, 0), glm::vec3(tj, 0));// (curToEndX * curToTargetY - curToEndY * curToTargetX) / endTargetMag;
			sinRotAng = signedCross.z;

			if (sinRotAng < 0)
			{
				rotAng = -rotAng;
			}

			switch (itr)
			{
			case 0:
				joint->SetRotateX(*joint->GetRotateX() + rotAng);
				break;
			case 1:
				joint->SetRotateY(*joint->GetRotateY() + rotAng);
				break;
			case 2:
				joint->SetRotateZ(*joint->GetRotateZ() + rotAng);
				break;
			}

			glm::vec3 newLeafPosition = leafBone->GetGlobalTipPosition();

			float newDistance = glm::length(newLeafPosition - target);

			// Stop if we are already there
			if (newDistance < arrivalDistance)
			{
				return;
			}
		}
	}

}