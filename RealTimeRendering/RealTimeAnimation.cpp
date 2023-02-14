#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <vector>
#include "Model.h"
#include <random>
#include "Camera.h"
#include "FixedCamera.h"
#include <irrKlang.h>
#include "Skybox.h"
#include <filesystem>
#include "Log.h"

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
int Width;
int Height;

using namespace std;
using namespace irrklang;


vector<Shader*> shaders;

ISoundEngine* SoundEngine;

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
	return glm::perspective(glm::radians(60.0f), (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, 300.0f);
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	auto timeValue = glutGet(GLUT_ELAPSED_TIME);
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
		s->SetUniformVec3("LightDirection", LightDirection);
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
		//m->SetShader(activeShader);
		// Mesh 8 is the propellor
		//m->RotateMeshZ(8, angleSpeed * 1000);
		//m->RotateMeshX(9, angleSpeed * 1000);
		m->Draw();
	}

	// Draw last
	skybox->Draw(*activeCamera->GetViewTransform(), &projection);

	glutPostRedisplay();
	glutSwapBuffers();
}

void LoadShaders()
{
	// Set up the shaders
	Shader* textureShader = new Shader("./textures.vert", "./textures.frag", true);

	Shader* blinnPhongShader = new Shader("./blinnPhongVertex.glsl", "./blinnPhongFragment.glsl", false);
	blinnPhongShader->SetUniform1f("specularExp", 64);

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
	cameraHeight = 10.0f;
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
	//for (int i = 0; i < shaders.size(); i++) 
	//{
	//	Model* teapot = new Model("./teapot.obj", glm::vec3(i * 70, 0, 0), shaders.at(i));
	//	models.push_back(teapot);
	//}


	LightBulb = new Model("./Models/Lightbulb/LightBulb.obj", LightPosition, shaders.at(0));
	models.push_back(LightBulb);

	skybox = new Skybox();

	SoundEngine = createIrrKlangDevice();

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
	//Log::Level = LogLevel::Info;
}


void SetLightPosition(glm::vec3 pos) {
	LightPosition = pos;
	LightBulb->SetPosition(LightPosition);
}

glm::vec3 GetLightPosition()
{
	return LightPosition;
}


void mousePress(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		break;
	case GLUT_MIDDLE_BUTTON:
		break;
	case GLUT_RIGHT_BUTTON:
		break;
	default:
		Log::WriteLog("Unhandled mouse input.", Debug);
		break;
	}
}

// function to allow keyboard control
// it's called a callback function and must be registerd in main() using glutKeyboardFunc();
// the functions must be of a specific format - see freeglut documentation
// similar functions exist for mouse control etc
void keyPress(unsigned char key, int x, int y)
{
	float theta = 10.0f;// glm::radians(10.0f);
	float angle = 0.0f;
	bool rotated = false;
	glm::mat4 rotate;
	switch (key) {
	case '1':
		break;
	case '2':
		break;
	case '3':
		break;
	case 'd':
		models.at(0)->RotateY(theta);
		break;
	case 'a':
		models.at(0)->RotateY(-theta);
		break;
	case 'w':
		models.at(0)->RotateX(theta);
		break;
	case 's':
		models.at(0)->RotateX(-theta);
		break;
	case 'q':
		models.at(0)->RotateZ(theta);
		break;
	case 'e':
		models.at(0)->RotateZ(-theta);
		break;
	case ' ':
		Pause = !Pause;
		if (Pause)
		{
			angleSpeed = 0.0;
		}
		else
		{
			angleSpeed = 0.01;
		}
		break;
	case '+':
		rotatationMeshIndex++;
		break;
	case '-':
		rotatationMeshIndex--;
		break;
	case '0':
		break;
	}

	// we must call these to redraw the scene after we make any changes 
	glutPostRedisplay();
}


//https://community.khronos.org/t/what-are-the-codes-for-arrow-keys-to-use-in-glut-keyboard-callback-function/26457/2
void altKeyPress(int key, int x, int y)
{
	int i = 0;
	glm::vec3 position;
	switch (key)
	{
	case GLUT_KEY_UP:
		cameraOrbit -= 10.0f;
		break;

	case GLUT_KEY_DOWN:
		cameraOrbit += 10.0f;
		break;
	case GLUT_KEY_LEFT:
		for (int j = 0; j < models.size(); j++)
		{
			if (models.at(j)->GetPosition() == defaultCamera.GetTarget())
			{
				i = j - 1;
				break;
			}
		}
		if (i < 0) {
			i = models.size() - 1;
		}
		defaultCamera.SetTarget(models.at(i)->GetPosition());
		break;
	case GLUT_KEY_RIGHT:
		for (int j = 0; j < models.size(); j++)
		{
			if (models.at(j)->GetPosition() == defaultCamera.GetTarget())
			{
				i = j + 1;
				break;
			}
		}
		if (i >= models.size()) {
			i = 0;
		}
		defaultCamera.SetTarget(models.at(i)->GetPosition());
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	Width = 800;
	Height = 600;
	// Set up the window
	glutInit(&argc, argv);
	glewExperimental = GL_TRUE;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Real Time Rendering");
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutMouseFunc(mousePress);
	glutSpecialFunc(altKeyPress);
	glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();

	// Begin infinite event loop
	glutMainLoop();
	return 0;
}