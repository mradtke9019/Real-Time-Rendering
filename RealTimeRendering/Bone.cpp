#include "Bone.h"

//Bone::Bone(Shader* s)
//{
//	LocalPosition = glm::vec3(0, 0, 0);
//	BoneModel = Model("./Models/Cylinder/cylinder.obj", LocalPosition,s);
//	parent = nullptr;
//	children = std::vector<Bone*>();
//	Target = glm::vec3(0, 0, 0);
//	MoveToTarget = false;
//	Idx = 0;
//}
//
//
//Bone::Bone(Shader* s, Bone* parent)
//{
//	LocalPosition = glm::vec3(0, 0, 0);
//	BoneModel = Model("./Models/Cylinder/cylinder.obj", LocalPosition, s);
//	this->parent = parent;
//	children = std::vector<Bone*>();
//	Target = glm::vec3(0, 0, 0);
//	MoveToTarget = false;
//	Idx = 0;
//}
//
//Bone::Bone(Shader* s, Bone* parent, glm::vec3 pos)
//{
//	LocalPosition = pos;
//	BoneModel = Model("./Models/Cylinder/cylinder.obj", LocalPosition, s);
//	this->parent = parent;
//	children = std::vector<Bone*>();
//	Target = glm::vec3(0, 0, 0);
//	MoveToTarget = false;
//	Idx = 0;
//}
//
///// <summary>
///// Sets the target that this bone and its children should move towards.
///// </summary>
///// <param name="t"></param>
//void Bone::SetTarget(glm::vec3 t)
//{
//	Target = t;
//}
//
//glm::vec3* Bone::GetTarget()
//{
//	return &Target;
//}
//
///// <summary>
///// Uses the current bone as the root and attempts to move itself as well as its children so that all of its children will try to move towards the target point.
///// Sources: https://www.ryanjuckett.com/cyclic-coordinate-descent-in-2d/ 
///// 
///// </summary>
///// <param name="target"></param>
///// <returns></returns>
//void Bone::IterateTowardsTarget()
//{
//	const float arrivalDistance = 0.5f;
//
//	Bone* root = this;
//	std::vector<Bone*> bones = std::vector<Bone*>();
//	bones.push_back(root);
//	int numBones = 1;
//
//	// Just do depth first layer of bones
//	Bone* joint = this->GetFirstChild();
//
//	while (joint != nullptr)
//	{
//		numBones++;
//		bones.push_back(joint);
//		joint = joint->GetFirstChild();
//	}
//
//	Bone* leafBone = bones[numBones - 1];
//	// Tip of our point e
//	glm::vec3 end = leafBone->GetGlobalTipPosition();
//
//	// Stop if we hvae reached our threshold
//	if (glm::length(end - Target) < arrivalDistance)
//	{
//		return;
//	}
//
//	// Search for 
//		// Iterate through each bone and move them slightly towards the target
//	for (int i = numBones - 1; i >= 0; --i)
//	{
//		// Imagine 3 different planes that we need to get to at the same time for each xy, xz, and yz plane
//		// Represent the current bone as joint j
//		joint = bones[i];
//		end = leafBone->GetGlobalTipPosition();
//
//		glm::vec3 j = joint->GetGlobalRootPosition();
//		glm::vec3 e = end;
//		glm::vec3 t = Target;
//
//		// Get the current bones difference from the leaf position
//		glm::vec3 ej = e - j;
//		ej = glm::normalize(ej);
//
//
//		glm::vec3 tj = t - j;
//		tj = glm::normalize(tj);
//
//		float cosRotAng;
//
//		cosRotAng = glm::dot(ej, tj);// Length of vectors should be 1 at this point, so do not need to divide by their magnitude, which is 1 anyway
//
//		double rotAng = glm::acos(glm::max(-1.0f, glm::min(1.0f, cosRotAng)));
//		rotAng = IRotatable::SimplifyAngle(rotAng);
//
//		// Axis to rotate on with cross product
//		glm::vec3 cross = glm::cross(ej, tj);
//
//		joint->SetAxis(cross);
//		joint->IncrementAxisAngle(rotAng);
//		joint->UseAxisAngle(true);
//
//		glm::vec3 newLeafPosition = leafBone->GetGlobalTipPosition();
//
//		float newDistance = glm::length(newLeafPosition - Target);
//		//joint->Draw(false);
//		// Stop if we are already there
//		if (newDistance < arrivalDistance)
//		{
//			return;
//		}
//	}
	

//}