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
#include "KnotShader.h"

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

IShader* activeShader;
int shaderChosen;
std::vector<string> shaderNames;
vector<IShader*> shaders;

ICamera* activeCamera;
FixedCamera defaultCamera;

vector<Model*> models;
Model* LightBulb;
Skybox* skybox;

vector<Texture> textures;

// Idea to use delta frame from here https://learnopengl.com/Getting-started/Camera
float deltaTime;// Time between current frame and last frame
float lastFrame; // Time of last frame
float currentFrame;
bool updateTime = true;

int rotatationMeshIndex = 0;

float cameraOrbit;
float cameraHeight;
float orbitSpeed;
float theta;
glm::vec3 CameraTarget;

//////////// Shader parameters
float specularExp;

// Tree properties
float rmin, rmax, hmax, knum;

glm::vec3 LightColor;
glm::vec3 LightPosition;


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
		IShader* s = shaders.at(i);

		s->SetUniformVec3("LightColor", LightColor);
		s->SetUniformVec3("LightPosition", LightPosition);
		if (updateTime)
		{
			s->SetUniform1f("time", timeValue);
		}
		s->SetUniform1f("rand", r);
		s->SetUniformMatrix4fv("view", defaultCamera.GetViewTransform());
		s->SetUniformMatrix4fv("projection", &projection);
		s->SetUniformVec3("cameraPos", defaultCamera.GetPosition());
		s->SetUniformVec3("viewPos", defaultCamera.GetPosition());
		s->SetUniformVec3("viewPosition", defaultCamera.GetPosition());
		s->SetUniformVec3("ViewPosition", defaultCamera.GetPosition());
	}

	for (int i = 0; i < models.size(); i++)
	{
		Model* m = models.at(i);
		m->SetShader(activeShader);
		m->Draw();
	}
}

void LoadTextures()
{
}

void LoadShaders()
{
	// Set up the shaders
	Shader* blinnPhongShader = new Shader("./blinnPhong.vert", "./blinnPhong.frag", false);
	blinnPhongShader->SetUniform1f("specularExp", 64);
	blinnPhongShader->SetUniform1i("UseNormalMap", 0);

	KnotShader* woodShader = new KnotShader("./Procedural.vert", "./Procedural.frag", false);

	KnotShader* woodShader2 = new KnotShader("./Procedural2.vert", "./Procedural2.frag", false);


	rmin = 104.06;
	rmax = 143.375;
	hmax = 4000;
	knum = 108;

	shaders.push_back(woodShader);
	shaders.push_back(woodShader2);
	shaders.push_back(blinnPhongShader);

	activeShader = woodShader;
	shaderChosen = 0;

	for (auto s : shaders)
	{
		// Set the Knot Shaders to be initialized with the same values
		if (dynamic_cast<KnotShader*>(s) != nullptr)
		{
			KnotShader* k = dynamic_cast<KnotShader*>(s);
			k->SetRMin(rmin);
			k->SetRMax(rmax);
			k->SetHMax(hmax);
			k->SetKNum(knum);
			k->SetUniform1i("N", 20);
		}
		shaderNames.push_back(s->GetShaderName());
	}
}


void LoadCameras()
{
	cameraOrbit = 1.7f;
	cameraHeight = 1.0f;
	orbitSpeed = 0.0f;
	theta = 0.0f;
	CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	defaultCamera = FixedCamera(glm::vec3(0.0f, cameraHeight, cameraOrbit), CameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	activeCamera = &defaultCamera;
}

void LoadObjects()
{
	Model* model = new Model("./Models/Plank/plank.obj", glm::vec3(0, 0, 0), activeShader);
	models.push_back(model);
}

void initLight()
{
	LightColor = glm::vec3(0.5, 0.5, 0.5);
	LightPosition = glm::vec3(20, 100.0, 0);
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0.749f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	initLight();
	LoadShaders();
	LoadCameras();
	LoadTextures();
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
	ImGui::SetNextWindowSize(ImVec2(400, 200));
	ImGui::Begin("ImGui");

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Checkbox("Update Time", &updateTime);

	if (ImGui::TreeNode("Camera Settings"))
	{
		ImGui::SliderFloat("Orbit Speed", &orbitSpeed, 0.0f, 100.0f);
		ImGui::SliderFloat("Camera Height", &cameraHeight, -20.0f, 20.0f);
		ImGui::SliderFloat("Orbit Radius", &cameraOrbit, 0.0f, 100.0f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Shader Settings"))
	{
		if (ImGui::BeginCombo("Active Shader", shaderNames[shaderChosen].c_str()))
		{

			for (int i = 0; i < shaders.size(); i++)
			{
				if (ImGui::Selectable(shaderNames[i].c_str(), i == shaderChosen))
				{
					activeShader = shaders[i];
					shaderChosen = i;
				}
			}

			ImGui::EndCombo();
		}

		ImGui::TreePop();
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