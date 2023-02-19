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
#include "Log.h"

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
int Width;
int Height;

using namespace std;


vector<Shader*> shaders;

ICamera* activeCamera;
FixedCamera defaultCamera;

bool Pause;

vector<Model*> models;
Model* airplane;
Model* gear;
Model* LightBulb;
Skybox* skybox;


// Idea to use delta frame from here https://learnopengl.com/Getting-started/Camera
float deltaTime;// Time between current frame and last frame
float lastFrame; // Time of last frame
float currentFrame;

int rotatationMeshIndex = 0;

float cameraOrbit;
float cameraHeight;
float angleSpeed;
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


	auto timeValue = glfwGetTime();
	currentFrame = timeValue;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	glm::mat4 projection = GetProjection();


	theta += angleSpeed * deltaTime;

	float x = cameraOrbit * cos(glm::radians(theta));
	float y = cameraOrbit * sin(glm::radians(theta));
	glm::vec3 cameraPosition = glm::vec3(x, cameraHeight, y);
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

	airplane->RotateMeshZ(29, angleSpeed * 1000);
	airplane->RotateMeshZ(30, angleSpeed * 1000);
	airplane->RotateMeshZ(31, angleSpeed * 1000);
	airplane->RotateMeshZ(32, angleSpeed * 1000);

	for (int i = 0; i < models.size(); i++)
	{
		Model* m = models.at(i);
		m->Draw();
	}

	// Draw last
	skybox->Draw(*activeCamera->GetViewTransform(), &projection);
}

void LoadShaders()
{
	// Set up the shaders
	Shader* textureShader = new Shader("./textures.vert", "./textures.frag", true);

	Shader* blinnPhongShader = new Shader("./blinnPhong.vert", "./blinnPhong.frag", false);
	blinnPhongShader->SetUniform1f("specularExp", 64);
	blinnPhongShader->SetUniform1i("UseNormalMap", 0);

	Shader* reflectionShader = new Shader("./fresnel.vert", "./reflection.frag");
	reflectionShader->LoadCubemap("cubemap");


	Shader* refractionShader = new Shader("./fresnel.vert", "./refraction.frag");
	refractionShader->LoadCubemap("cubemap");

	Shader* chromaticShader = new Shader("./fresnel.vert", "./chromatic.frag");
	chromaticShader->LoadCubemap("cubemap");

	Shader* fresnelShader = new Shader("./fresnel.vert", "./fresnel.frag", false);
	fresnelShader->LoadCubemap("cubemap");


	shaders.push_back(blinnPhongShader);

}


void LoadCameras()
{
	cameraOrbit = 100.0f;
	cameraHeight = 5.0f;
	angleSpeed = 0.01;
	theta = 0.0f;
	CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	defaultCamera = FixedCamera(glm::vec3(0.0f, cameraHeight, cameraOrbit), CameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	activeCamera = &defaultCamera;
}

void LoadObjects()
{

	airplane = new Model("./Models/Airplane/piper_pa18.obj", glm::vec3(0, 0, 0), shaders.at(0));

	// Positions of the propellor and the wheels
	airplane->meshes.at(29).SetOrigin(glm::vec3(0.0f, 1.23, 3.37f), airplane->GetPosition());
	airplane->meshes.at(30).SetOrigin(glm::vec3(0.0f, 1.23, 3.37f), airplane->GetPosition());
	airplane->meshes.at(31).SetOrigin(glm::vec3(0.0f, 1.23, 3.37f), airplane->GetPosition());
	airplane->meshes.at(32).SetOrigin(glm::vec3(0.0f, 1.23, 3.37f), airplane->GetPosition());

	models.push_back(airplane);


	gear = new Model("./Models/Gear/Gear2.obj", glm::vec3(100, 0, 0), shaders.at(0));
	models.push_back(gear);


	LightBulb = new Model("./Models/Sphere/Sphere.obj", LightPosition, shaders.at(0));
	models.push_back(LightBulb);

	skybox = new Skybox();


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


int main()
{
	Width = 800;
	Height = 600;
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
		display(window);


		//ImGui new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowSize(ImVec2(200, 200));
		ImGui::Begin("ImGUI window");


		//ImGui::SliderFloat("pitch", &rotations.x, -360.0f, 360.0f);
		//ImGui::SliderFloat("yaw", &rotations.y, -360.0f, 360.0f);
		//ImGui::SliderFloat("roll", &rotations.z, -360.0f, 360.0f);
		//ImGui::SliderInt("meshNum", &meshToDraw, 0, 40);



		/*
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		*/





		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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


//int main(int argc, char** argv) {
//	Width = 800;
//	Height = 600;
//	// Set up the window
//	glutInit(&argc, argv);
//	glewExperimental = GL_TRUE;
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//	glutInitWindowSize(Width, Height);
//	glutCreateWindow("Real Time Rendering");
//	// Tell glut where the display function is
//	glutDisplayFunc(display);
//	glutKeyboardFunc(keyPress);
//	glutMouseFunc(mousePress);
//	glutSpecialFunc(altKeyPress);
//	glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//	// A call to glewInit() must be done after glut is initialized!
//	GLenum res = glewInit();
//	// Check for any errors
//	if (res != GLEW_OK) {
//		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
//		return 1;
//	}
//	// Set up your objects and shaders
//	init();
//
//	// Begin infinite event loop
//	glutMainLoop();
//	return 0;
//}