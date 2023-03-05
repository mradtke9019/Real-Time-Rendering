#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

#include "Model.h"
#include <random>
#include "Camera.h"
#include "FixedCamera.h"
#include "Skybox.h"
#include <filesystem>
#include "Bone.h"


int Width;
int Height;
using namespace std;

Shader* activeShader;
vector<Shader*> shaders;

ICamera* activeCamera;
FixedCamera defaultCamera;

bool MoveToTarget;

std::vector<glm::vec3> Targets;

bool drawStructure1;
bool drawStructure2;

std::vector<Model*> SphereTargets;

glm::vec3 Target;
Model* SphereTarget;
Bone* Root;
Bone* Structure2;
vector<Model*> models;
Model* LightBulb;
Skybox* skybox;


// Idea to use delta frame from here https://learnopengl.com/Getting-started/Camera
float deltaTime;// Time between current frame and last frame
float lastFrame; // Time of last frame
float currentFrame;

int rotatationMeshIndex = 0;

float cameraOrbit;
float cameraHeight;
float orbitSpeed;
float theta;
glm::vec3 CameraTarget;

//////////// Shader parameters
float specularExp;


glm::vec3 LightColor;
glm::vec3 LightPosition;
glm::vec3 LightDirection;


glm::mat4 GetProjection()
{
	return glm::perspective(glm::radians(60.0f), (float)Width / (float)Height, 0.1f, 300.0f);
}


void display(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	float timeValue = static_cast<float>(glfwGetTime());
	currentFrame = timeValue;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	glm::mat4 projection = GetProjection();


	theta += orbitSpeed * deltaTime;

	float x = cameraOrbit * cos(glm::radians(theta));
	float y = cameraOrbit * sin(glm::radians(theta));
	glm::vec3 cameraPosition = glm::vec3(x, cameraHeight, y);

	//defaultCamera.SetTarget(models.at(modelFocused)->GetPosition());
	defaultCamera.SetPosition(cameraPosition + defaultCamera.GetTarget());

	for (int i = 0; i < shaders.size(); i++)
	{
		Shader* s = shaders.at(i);

		s->SetUniformVec3("LightColor", LightColor);
		s->SetUniformVec3("LightPosition", LightPosition);
		s->SetUniform1f("time", timeValue);
		s->SetUniform1f("rand", r);
		s->SetUniformMatrix4fv("view", defaultCamera.GetViewTransform());
		s->SetUniformMatrix4fv("projection", &projection);
		s->SetUniformVec3("cameraPos", defaultCamera.GetPosition());
	}
	
	for (int i = 0; i < models.size(); i++)
	{
		Model* m = models.at(i);
		m->Draw();
	}

	if (drawStructure1)
	{
		Root->Draw();
	}
	if (drawStructure2)
	{
		Structure2->Draw();
	}
	
	for (int i = 0; i < SphereTargets.size(); i++)
	{
		SphereTargets[i]->Draw();
	}
	//SphereTarget->Draw();

	//cube->Draw();

	// Draw last
	//skybox->Draw(*activeCamera->GetViewTransform(), &projection);
}

void LoadShaders()
{
	// Set up the shaders
	//Shader* textureShader = new Shader("./simple.vert", "./simple.frag", true);

	Shader* blinnPhongShader = new Shader("./blinnPhong.vert", "./blinnPhong.frag", false);
	blinnPhongShader->SetUniform1f("specularExp", 64);
	blinnPhongShader->SetUniform1i("UseNormalMap", 0);

	//Shader* reflectionShader = new Shader("./fresnel.vert", "./reflection.frag");
	////reflectionShader->LoadCubemap("cubemap");


	//Shader* refractionShader = new Shader("./fresnel.vert", "./refraction.frag");
	////refractionShader->LoadCubemap("cubemap");

	//Shader* chromaticShader = new Shader("./fresnel.vert", "./chromatic.frag");
	////chromaticShader->LoadCubemap("cubemap");

	//Shader* fresnelShader = new Shader("./fresnel.vert", "./fresnel.frag", false);
	//fresnelShader->LoadCubemap("cubemap");


	shaders.push_back(blinnPhongShader);
	activeShader = blinnPhongShader;
}


void LoadCameras()
{
	cameraOrbit = 15.0f;
	cameraHeight = 10.0f;
	orbitSpeed = 0.0f;
	theta = 0.0f;
	CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	defaultCamera = FixedCamera(glm::vec3(0.0f, cameraHeight, cameraOrbit), CameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	activeCamera = &defaultCamera;
}

void LoadObjects()
{

	for (int i = 0; i < 1; i++)
	{
		//Model* model = new Model("./Models/Airplane/piper_pa18.obj", glm::vec3(i * 50, 0, 0), shaders.at(0), mimmaps[i]);
	}

	//Model* model = new Model("./Models/Cube/cube.obj", glm::vec3(0, 0, 0), shaders.at(0), mipmapsDisplay[MIN_MIMAPidx]);
	//models.push_back(model);

	Root = new Bone(activeShader);
	Root->AddBone(new Bone(activeShader));
	Root->GetChildren().at(0)->AddBone(new Bone(activeShader));
	Root->GetChildren().at(0)->GetChildren().at(0)->AddBone(new Bone(activeShader));
	Root->GetChildren().at(0)->GetChildren().at(0)->GetChildren().at(0)->AddBone(new Bone(activeShader));
	int depth = Bone::GetTreeHeight(Root);
	
	Targets.push_back(glm::vec3(2, 0, 2));
	Targets.push_back(glm::vec3(2, 0, -2));
	Targets.push_back(glm::vec3(-2, 0, 2));
	Targets.push_back(glm::vec3(-2, 0, -2));

	for (int i = 0; i < 4; i++)
	{
		SphereTargets.push_back(new Model("./Models/Sphere/Sphere.obj", Targets[i], activeShader));
		//Target = glm::vec3(0, 0, 1);
		//SphereTarget = new Model("./Models/Sphere/Sphere.obj", Target, activeShader);
		//MoveToTarget = false;
	}


	Structure2 = new Bone(activeShader);
	Structure2->AddBone(new Bone(activeShader));
	Structure2->GetChildren().at(0)->AddBone(new Bone(activeShader));
	Structure2->GetChildren().at(0)->GetChildren().at(0)->AddBone(new Bone(activeShader));
	Structure2->GetChildren().at(0)->GetChildren().at(0)->GetChildren().at(0)->AddBone(new Bone(activeShader));

	Structure2->AddBone(new Bone(activeShader));
	Structure2->GetChildren().at(1)->AddBone(new Bone(activeShader));
	Structure2->GetChildren().at(1)->GetChildren().at(0)->AddBone(new Bone(activeShader));
	Structure2->GetChildren().at(1)->GetChildren().at(0)->GetChildren().at(0)->AddBone(new Bone(activeShader));


	drawStructure1 = true;
	drawStructure2 = false;
}

void initLight()
{
	LightColor = glm::vec3(0.5, 0.5, 0.5);
	LightPosition = glm::vec3(20, 100.0, 0);
	LightDirection = glm::vec3(0.0, -1.0, 0.0);
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0.749f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	initLight();
	LoadShaders();
	LoadCameras();
	LoadObjects();
}


void SetLightPosition(glm::vec3 pos) {
	LightPosition = pos;
	LightBulb->SetPosition(LightPosition);
}

glm::vec3 GetLightPosition()
{
	return LightPosition;
}


// function to allow keyboard control
// it's called a callback function and must be registerd in main() using glutKeyboardFunc();
// the functions must be of a specific format - see freeglut documentation
// similar functions exist for mouse control etc
void keyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Height = height;
	Width = width;
	glViewport(0, 0, width, height);
}

// Function where we set our debuggables
void ImguiData()
{
	//ImGui new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowSize(ImVec2(400,200));
	ImGui::Begin("ImGui");

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::TreeNode("Camera Settings"))
	{
		ImGui::SliderFloat("Orbit Speed", &orbitSpeed, 0.0f, 100.0f);
		ImGui::SliderFloat("Camera Height", &cameraHeight, -20.0f, 20.0f);
		ImGui::SliderFloat("Orbit Radius", &cameraOrbit, 0.0f, 100.0f);

		ImGui::TreePop();
	}

	// Target Settings
	if (ImGui::TreeNode("Target Settings"))
	{
		for (int i = 0; i < SphereTargets.size(); i++)
		{
			ImGui::Text(std::string("Target " + std::to_string(i)).c_str());
			ImGui::SliderFloat(std::string("X" + std::to_string(i)).c_str(), &(Targets[i].x), -10.0f, 10.0f);
			ImGui::SliderFloat(std::string("Y" + std::to_string(i)).c_str(), &(Targets[i].y), -10.0f, 10.0f);
			ImGui::SliderFloat(std::string("Z" + std::to_string(i)).c_str(), &(Targets[i].z), -10.0f, 10.0f);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Structure Options"))
	{
		ImGui::Checkbox(std::string("Show Structure 1").c_str(), &drawStructure1);
		ImGui::Checkbox(std::string("Show Structure 2").c_str(), &drawStructure2);
		ImGui::TreePop();
	}

	for (int i = 0; i < SphereTargets.size(); i++)
	{
		SphereTargets[i]->SetPosition(Targets[i]);
	}
	//ImGui::Checkbox("Move Root To Target",&MoveToTarget);
	
	if (drawStructure1)
	{
		ImGui::Text("Structure 1");
		Bone* curr = Root;
		int i = 0;
		while (curr != nullptr)
		{
			if (ImGui::TreeNode(("Bone " + std::to_string(i)).c_str()))
			{
				glm::vec3 rootPos = curr->GetGlobalRootPosition();
				glm::vec3 pos = curr->GetGlobalTipPosition();
				ImGui::Text(("Root Global Position: (" + std::to_string(rootPos.x) + "," + std::to_string(rootPos.y) + "," + std::to_string(rootPos.z) + ")").c_str());
				ImGui::Text(("Tip Global Position: (" + std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(pos.z) + ")").c_str());

				ImGui::SliderFloat("X", curr->GetRotateX(), 0.0f, 180.f);
				ImGui::SliderFloat("Y", curr->GetRotateY(), 0.0f, 180.f);
				ImGui::SliderFloat("Z", curr->GetRotateZ(), 0.0f, 180.f);


				ImGui::SliderInt("Target Index", curr->GetIdx(), 0, 3);
				ImGui::Checkbox("Move To Target", curr->MovingToTarget());
				ImGui::TreePop();
			}

			curr->SetTarget(Targets[*curr->GetIdx()]);

			// Create rotations for each branch in the bone structure
			std::vector<Bone*> children = curr->GetChildren();
			if (children.size() > 0)
			{
				curr = children.at(0);
			}
			else
			{
				curr = nullptr;
			}
			i++;
		}
	}
	else if (drawStructure2)
	{
		ImGui::Text("Structure 2");
		std::vector<Bone*> tree2 = std::vector<Bone*>();
		tree2.push_back(Structure2);
		for (auto x : Structure2->GetAllBones())
		{
			tree2.push_back(x);
		}

		int layer = 0;
		for (auto curr : tree2)
		{
			std::string l = std::to_string(layer);
			if (ImGui::TreeNode(("L" + std::to_string(curr->GetDepth()) + " Bone " + l).c_str()))
			{
				glm::vec3 rootPos = curr->GetGlobalRootPosition();
				glm::vec3 pos = curr->GetGlobalTipPosition();
				ImGui::Text(("Root Global Position: (" + std::to_string(rootPos.x) + "," + std::to_string(rootPos.y) + "," + std::to_string(rootPos.z) + ")").c_str());
				ImGui::Text(("Tip Global Position: (" + std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(pos.z) + ")").c_str());

				ImGui::SliderFloat("X", curr->GetRotateX(), 0.0f, 180.f);
				ImGui::SliderFloat("Y", curr->GetRotateY(), 0.0f, 180.f);
				ImGui::SliderFloat("Z", curr->GetRotateZ(), 0.0f, 180.f);

				ImGui::SliderInt("Target Index", curr->GetIdx(), 0, 3);
				ImGui::Checkbox("Move To Target", curr->MovingToTarget());
				ImGui::TreePop();
			}

			curr->SetTarget(Targets[*curr->GetIdx()]);
			layer++;
		}
	}

}

void ImguiDraw()
{
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main()
{
	Width = 1200;
	Height = 900;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// --------------------
	GLFWwindow* window = glfwCreateWindow(Width, Height, "Scene", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyPress);
	// For potential future use
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//IMGUI code
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	init();


	while (!glfwWindowShouldClose(window))
	{

		ImguiData();
		display(window);
		ImguiDraw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}