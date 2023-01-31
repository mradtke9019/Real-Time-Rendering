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

//////////// Shader parameters
float thresholds[3];
float specularExp;
// cook torrance
float F0 = 0.8;
float roughness = 0.1;
float k = 0.2;

glm::vec3 LightColor;
glm::vec3 LightPosition; 
glm::vec3 LightDirection;


glm::mat4 GetProjection()
{
	return glm::perspective(glm::radians(60.0f), (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, 300.0f);
}

// https://stackoverflow.com/questions/538661/how-do-i-draw-text-with-glut-opengl-in-c
void RenderString(float x, float y, void* font, const unsigned char* string, glm::vec3 rgb)
{
	//unsigned char string[] = "The quick god jumps over the lazy brown fox."
	//	int w;
	//w = glutBitmapLength(GLUT_BITMAP_8_BY_13, string);
	char* c;

	glColor3f(rgb.r, rgb.g, rgb.b);
	//glRasterPos2f(x,y
	glRasterPos2i(x,y);

	glutBitmapString(font, string);
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
	//glm::mat4 identity = glm::mat4(1.0);
	//activeShader->SetUniformMatrix4fv("model", &identity);
	//activeShader->SetUniformMatrix4fv("projection", &identity);
	
	//for (int i = -10; i < 10; i++)
	//{
	//	RenderString(i, i, GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)"Hello World", glm::vec3(1.0f, 0.0f, 0.0f));
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

	thresholds[0] = 0.7;
	thresholds[1] = 0.5;
	thresholds[2] = 0.25;
	nonRealisticShader.SetUniformFloatArray("thresholds", thresholds, 3);
	
	specularExp = 64;
	blinnPhongShader.SetUniform1f("specularExp", specularExp);

	//float F0 = 0.8;
	//float roughness = 0.1;
	//float k = 0.2;
	realisticShader.SetUniform1f("F0", F0);
	realisticShader.SetUniform1f("roughness", roughness);
	realisticShader.SetUniform1f("k", k);


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


void IncrementShaders() {

	//shaders.push_back(blinnPhongShader);
	//shaders.push_back(nonRealisticShader);
	//shaders.push_back(realisticShader);
	thresholds[0] += 0.1;
	thresholds[1] += 0.1;
	thresholds[2] += 0.1;
	shaders.at(1).SetUniformFloatArray("thresholds", thresholds, 3);


	specularExp = specularExp * 2;
	shaders.at(0).SetUniform1f("specularExp", specularExp);

	F0 += 0.1;
	roughness += 0.1;
	k += 0.1;
	shaders.at(2).SetUniform1f("F0", F0);
	shaders.at(2).SetUniform1f("roughness", roughness);
	shaders.at(2).SetUniform1f("k", k);
}
void decrementShaders() {

	//shaders.push_back(blinnPhongShader);
	//shaders.push_back(nonRealisticShader);
	//shaders.push_back(realisticShader);
	thresholds[0] -= 0.1;
	thresholds[1] -= 0.1;
	thresholds[2] -= 0.1;
	shaders.at(1).SetUniformFloatArray("thresholds", thresholds, 3);


	specularExp = specularExp / 2;
	shaders.at(0).SetUniform1f("specularExp", specularExp);


	F0 -= 0.1;
	roughness -= 0.1;
	k -= 0.1;
	shaders.at(2).SetUniform1f("F0", F0);
	shaders.at(2).SetUniform1f("roughness", roughness);
	shaders.at(2).SetUniform1f("k", k);

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
		std::cout << "Unhandled mouse input" <<std::endl;
		break;
	}
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
	case 'w':
		IncrementShaders();
		break;
	case 's':
		decrementShaders();
		break;
	case ' ':
		Pause = !Pause;
		if (Pause)
		{
			angleSpeed = 0.0;
		}
		else
		{
			angleSpeed = 0.03;
		}
		break;
	}

	// we must call these to redraw the scene after we make any changes 
	glutPostRedisplay();
}


//https://community.khronos.org/t/what-are-the-codes-for-arrow-keys-to-use-in-glut-keyboard-callback-function/26457/2
void altKeyPress(int key, int x, int y)
{
	glm::vec3 position;
	switch (key)
	{
	case GLUT_KEY_UP:
		position = GetLightPosition();
		position.z -= 10;
		SetLightPosition(position);
		break;
		break;
	case GLUT_KEY_DOWN:
		position = GetLightPosition();
		position.z += 10;
		SetLightPosition(position);
		break;
	case GLUT_KEY_LEFT:
		position = GetLightPosition();
		position.x += 10;
		SetLightPosition(position);
		break;
	case GLUT_KEY_RIGHT:
		position = GetLightPosition();
		position.x -= 10;
		SetLightPosition(position);
		break;
	}
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