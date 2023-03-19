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
int shaderChosen;
std::vector<string> shaderNames;
vector<Shader*> shaders;

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
		m->SetShader(activeShader);
		m->Draw(textures);
	}
}

void LoadTextures()
{
	textures = std::vector<Texture>();

	//uniform sampler2D ColorMap;
	//uniform sampler2D SpecularMap;
	//uniform sampler2D NormalMap;
	//uniform sampler2D PithRadiusMap;
	//uniform sampler2D KnotHeightMap;
	//uniform sampler2D KnotOrientMap;
	//uniform sampler2D KnotStateMap;

	// Tree geo maps
	Texture knot_height_map = Texture();
	knot_height_map.id = Texture::TextureFromFile("knot_height_map.bmp", "./Images/tree_geo_maps");
	knot_height_map.name = "KnotHeightMap";
	textures.push_back(knot_height_map);

	Texture knot_orientation_map = Texture();
	knot_orientation_map.id = Texture::TextureFromFile("knot_orientation_map.bmp", "./Images/tree_geo_maps");
	knot_orientation_map.name = "KnotOrientMap";
	textures.push_back(knot_orientation_map);

	Texture knot_state_map = Texture();
	knot_state_map.id = Texture::TextureFromFile("knot_state_map.bmp", "./Images/tree_geo_maps");
	knot_state_map.name = "KnotStateMap";
	textures.push_back(knot_state_map);

	Texture pith_and_radius_map = Texture();
	pith_and_radius_map.id = Texture::TextureFromFile("pith_and_radius_map.bmp", "./Images/tree_geo_maps");
	pith_and_radius_map.name = "PithRadiusMap";
	textures.push_back(pith_and_radius_map);


	// Wood color maps
	Texture wood_bar_color = Texture();
	wood_bar_color.id = Texture::TextureFromFile("wood_bar_color.bmp", "./Images/wood_color_maps");
	wood_bar_color.name = "ColorMap";
	textures.push_back(wood_bar_color);

	Texture wood_bar_normal = Texture();
	wood_bar_normal.id = Texture::TextureFromFile("wood_bar_normal.bmp", "./Images/wood_color_maps");
	wood_bar_normal.name = "NormalMap";
	textures.push_back(wood_bar_normal);

	Texture wood_bar_specular = Texture();
	wood_bar_specular.id = Texture::TextureFromFile("wood_bar_specular.bmp", "./Images/wood_color_maps");
	wood_bar_specular.name = "SpecularMap";
	textures.push_back(wood_bar_specular);
}

void LoadShaders()
{
	// Set up the shaders
	Shader* blinnPhongShader = new Shader("./blinnPhong.vert", "./blinnPhong.frag", false);
	blinnPhongShader->SetUniform1f("specularExp", 64);
	blinnPhongShader->SetUniform1i("UseNormalMap", 0);

	Shader * woodShader = new Shader("./Procedural.vert", "./Procedural.frag", false);


	shaders.push_back(blinnPhongShader);
	shaders.push_back(woodShader);
	activeShader = blinnPhongShader;
	shaderChosen = 0;

	for (auto s : shaders)
	{
		shaderNames.push_back(s->GetShaderName());
	}
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
	Model* model = new Model("./Models/Plank/plank.obj", glm::vec3(0, 0, 0), activeShader);
	models.push_back(model);

	rmin = 104.06; 
	rmax = 143.375;
	hmax = 4000;
	knum = 108;

	for (auto s : shaders)
	{
		s->SetUniform1f("rmin", rmin);
		s->SetUniform1f("rmax", rmax);
		s->SetUniform1f("hmax", hmax);
		s->SetUniform1f("knum", knum);
	}
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