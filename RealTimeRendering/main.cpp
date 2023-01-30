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
#include "Object.h"
#include <vector>
#include "Model.h"
#include <random>
#include "Camera.h"
#include "FixedCamera.h"
#include "Player.h"
#include "Ghost.h"
#include "Arena.h"
#include <irrKlang.h>
// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
int Width;
int Height;

using namespace std;
using namespace irrklang;

Shader* activeShader;
vector<Shader> shaders;

ISoundEngine* SoundEngine;

ICamera* activeCamera;

FixedCamera defaultCamera;

bool Pause;

vector<Model*> models;
vector<Model> teapots;
Model* LightBulb;


// Idea to use delta frame from here https://learnopengl.com/Getting-started/Camera
float deltaTime;// Time between current frame and last frame
float lastFrame; // Time of last frame
float currentFrame;

float cameraOrbit;
float cameraHeight;
float angleSpeed;
float theta;

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
	glm::vec3 cameraPosition = glm::vec3(x,cameraHeight,y);
	activeCamera->SetPosition(cameraPosition);


	for (int i = 0; i < shaders.size(); i++)
	{
		Shader* s = &shaders.at(i);

		s->SetUniformVec3("LightColor", LightColor);
		s->SetUniformVec3("LightPosition", LightPosition);
		s->SetUniformVec3("LightDirection", LightDirection);
		s->SetUniformVec3("LightDirection", cameraPosition);
		s->SetUniform1f("time", timeValue);
		s->SetUniform1f("rand", r);
		s->SetUniformMatrix4fv("view", activeCamera->GetViewTransform());
		s->SetUniformMatrix4fv("projection", &projection);
	}

	for (int i = 0; i < models.size(); i++)
	{
		Model* m = models.at(i);
		m->SetShader(activeShader);
		m->Draw();
	}
	//for (int i = 0; i < teapots.size(); i++) 
	//{
	//	Model* t = &teapots.at(i);
	//	t->SetShader(activeShader);
	//	t->Draw();
	//}

	glutPostRedisplay();
    glutSwapBuffers();
}

void LoadShaders()
{
	Shader realisticShader; // 
	Shader nonRealisticShader; // https://www.lighthouse3d.com/tutorials/glsl-12-tutorial/toon-shader-version-ii/
	Shader blinnPhongShader; // https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
	// Set up the shaders
	blinnPhongShader = Shader("./blinnPhongVertex.glsl", "./blinnPhongFragment.glsl");
	nonRealisticShader = Shader("./nonRealisticVertex.glsl", "./nonRealisticFragment.glsl");
	realisticShader = Shader("./realisticVertex.glsl", "./realisticFragment.glsl");


	shaders.push_back(blinnPhongShader);
	shaders.push_back(nonRealisticShader);
	shaders.push_back(realisticShader);
	activeShader = &shaders.at(0);
}


void LoadCameras()
{
	cameraOrbit = 150.0f;
	cameraHeight = 50.0f;
	angleSpeed = 0.03;
	theta = 0.0f;
	defaultCamera = FixedCamera(glm::vec3(0.0f, cameraHeight, cameraOrbit), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	activeCamera = &defaultCamera;
}

void LoadObjects()
{
	for (int i = -1; i < 2; i++)
	{
		glm::vec3 randomColor = glm::vec3(
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		glm::vec3 color  = glm::vec3(1, 1, 1);
		glm::vec3 position = glm::vec3(i * 70, 0, 0);


		if (i == -1)
		{
			color = glm::vec3(1,0,0);
		}
		else if (i == 0)
		{
			color = glm::vec3(0, 1, 0);
		}
		else if (i == 1)
		{
			color = glm::vec3(0, 0, 1);
		}
		Model* m = new Model("./teapot.obj", position, activeShader, color);

		//teapots.push_back(m);
		models.push_back(m);
	}


	LightBulb = new Model("./LightBulb.obj", LightPosition, activeShader, glm::vec3(1.0,1.0,1.0));
	models.push_back(LightBulb);

	SoundEngine = createIrrKlangDevice();
}

void initLight()
{
	LightColor = glm::vec3(0.5, 0.5, 0.5);
	LightPosition = glm::vec3(0, 50.0, 0);
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



// function to allow keyboard control
// it's called a callback function and must be registerd in main() using glutKeyboardFunc();
// the functions must be of a specific format - see freeglut documentation
// similar functions exist for mouse control etc
void keyPress(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		activeShader = &shaders.at(0);
		std::cout << "Shader 1" << std::endl;
		break;
	case '2':
		activeShader = &shaders.at(1);
		std::cout << "Shader 2" << std::endl;
		break;
	case '3':
		activeShader = &shaders.at(2);
		std::cout << "Shader 3" << std::endl;
		break;
	}

	// we must call these to redraw the scene after we make any changes 
	glutPostRedisplay();
}

int main(int argc, char** argv){
	Width = 800;
	Height = 600;
	// Set up the window
	glutInit(&argc, argv);
	glewExperimental = GL_TRUE;
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(Width, Height);
    glutCreateWindow("Real Time Rendering"); 
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress); 
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