#include "Bone.h"

Bone::Bone(Shader* s)
{
	Transform = glm::mat4(1);
	BoneModel = Model("./Cylinder/cylinder.obj",glm::vec3(0,0,0),s);
	parent = nullptr;
}


Bone::Bone(Shader* s, Bone* parent)
{
	Transform = glm::mat4(1);
	BoneModel = Model("./Cylinder/cylinder.obj", glm::vec3(0, 0, 0), s);
	this->parent = parent;
}