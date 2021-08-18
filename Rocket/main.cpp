// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;
using namespace glm;
#include <cmath>
#include <ctime>
#include <cstdlib>
#include<SOIL2.h>
#include"Camera.h"
#include"Shader.h"
#include"Texture.h"
#include "meshanim.h"
#include "mesh.h"
#include "Header.h"
#include "Person.h"
#include "Model.h"
#include "ModelAnim.h"
#include <windows.h>
#include <mmsystem.h>
#include "audiere.h"
using namespace audiere;
#if defined(_MSC_VER)
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "shlwapi.lib")
#endif 

bool keys[1024];
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool firstMouse = true;
glm::vec3 lightPos(0.0f, 10.0f, 0.0f);
GLfloat lastX = 400, lastY = 400;
//Camera per(glm::vec3(0.0f, 1.5, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
Person per;
bool Ball = false;
bool Music = false;
bool Music_start = false;
bool compare(Person& per, Mod& dom)
{
	//cout << "1 x = " << per.Position.x << "y = " << per.Position.y << "z = " << per.Position.z << endl;
	glm::vec3  lolpos = per.Position;
	glm::vec3 tempPos[8];
	for (int i = 0; i < 8; i++)
		tempPos[i] = dom.vectors[i];
	if (lolpos.y >= tempPos[7].y || lolpos.y + per.hight <= tempPos[0].y)
		return 0;
	glm::vec2 lolPosA;
	lolPosA.x = lolpos.x;
	lolPosA.y = lolpos.z;
	GLboolean D = 0;
	GLint num = 0;
	GLint count = 0;
	GLfloat dis;
	glm::vec2 lolPosB;
	glm::vec2 lolPosC;
	for (int i = 0; i < 4; i++)
	{
		lolPosB.x = tempPos[i].x;
		lolPosB.y = tempPos[i].z;
		lolPosC.x = tempPos[(i + 1) % 4].x;
		lolPosC.y = tempPos[(i + 1) % 4].z;
		dis = glm::distance(lolPosA, lolPosB);
		if (dis < per.radius)
			return 1;
		if ((lolPosA.x - lolPosB.x)*(lolPosC.y - lolPosB.y) - (lolPosA.y - lolPosB.y)*(lolPosC.x - lolPosB.x) > 0.0f)
		{
			D = 0;
			num = i;
			count++;
		}
	}
	if (D) 
		return 1;

	if (count > 1)
		return 0;

	lolPosB.x = tempPos[num].x;
	lolPosB.y = tempPos[num].z;
	lolPosC.x = tempPos[(num + 1) % 4].x;
	lolPosC.y = tempPos[(num + 1) % 4].z;
	if (glm::abs((lolPosB.x - lolPosA.x)*(lolPosC.y - lolPosA.y) - (lolPosC.x - lolPosA.x)*(lolPosB.y - lolPosA.y)) / glm::distance(lolPosB, lolPosC) >= per.radius)
		return 0;
	else
		return 1;
	return 0;
}
bool compareround(Person& per, glm::vec3 dot, GLfloat radius, GLfloat hight)
{
	if (per.Position.y >= dot.y + hight || per.Position.y + per.hight <= dot.y)
		return 0;
	glm::vec2 lolPosA;
	lolPosA.x = per.Position.x;
	lolPosA.y = per.Position.z;
	glm::vec2 lolPosB;
	lolPosB.x = dot.x;
	lolPosB.y = dot.z;
	if (glm::distance(lolPosA, lolPosB) < per.radius + radius)
		return 1;
	return 0;
}

bool compare(Person& per, ModAnim& dom)
{
	//cout << "DOM x = " << dom.Position.x << " y = " << dom.Position.y << " z = " << dom.Position.z << endl;

	//cout <<endl<< "Pos x" << per.Position.x << " osp y" << per.Position.y << " pos z " << per.Position.z << endl<<endl;
	glm::vec3  lolpos = per.Position;
	glm::vec3 tempPos[8];
	for (int i = 0; i < 8; i++)
		tempPos[i] = dom.vectors[i];
	if (lolpos.y >= tempPos[7].y || lolpos.y + per.hight <= tempPos[0].y)
		return 0;
	glm::vec2 lolPosA;
	//cout << "vec x" << tempPos[0].x << " vec y" << tempPos[0].y << " vec z " << tempPos[0].z << endl;

	lolPosA.x = lolpos.x;
	lolPosA.y = lolpos.z;
	GLboolean D = 0;
	GLint num = 0;
	GLint count = 0;
	GLfloat dis;
	glm::vec2 lolPosB;
	glm::vec2 lolPosC;
	//cout << "##################################################" << endl;
	for (int i = 0; i < 4; i++)
	{
		lolPosB.x = tempPos[i].x;
		lolPosB.y = tempPos[i].z;
		lolPosC.x = tempPos[(i + 1) % 4].x;
		lolPosC.y = tempPos[(i + 1) % 4].z;
		dis = glm::distance(lolPosA, lolPosB);
		if (dis < per.radius)
		{
		//	cout << "kek" << endl;
			return 1;
		}
		if ((lolPosA.x - lolPosB.x) * (lolPosC.y - lolPosB.y) - (lolPosA.y - lolPosB.y) * (lolPosC.x - lolPosB.x) > 0.0f)
		{
			D = 0;
			num = i;
			count++;
		}
		//cout << "D " << i << " " << (i + 1) % 4 << " " << (lolPosA.x - lolPosB.x)*(lolPosC.y - lolPosB.y) - (lolPosA.y - lolPosB.y)*(lolPosC.x - lolPosB.x) <<  endl;
		//cout << "lolPosB " << lolPosB.x << " " << lolPosB.y << endl;
		//cout << "lolPosC " << lolPosC.x << " " << lolPosC.y << endl;
	}
	if (D) return 1;
	if (count > 1) return 0;
	lolPosB.x = tempPos[num].x;
	lolPosB.y = tempPos[num].z;
	lolPosC.x = tempPos[(num + 1) % 4].x;
	lolPosC.y = tempPos[(num + 1) % 4].z;
	//GLfloat dis = glm::distance(lolPosB, lolPosC);
	//GLfloat one = glm::abs((lolPosB.x - lolPosA.x)*(lolPosC.y - lolPosA.y) - (lolPosC.x - lolPosA.x)*(lolPosB.y - lolPosA.y));
	if (glm::abs((lolPosB.x - lolPosA.x) * (lolPosC.y - lolPosA.y) - (lolPosC.x - lolPosA.x) * (lolPosB.y - lolPosA.y)) / glm::distance(lolPosB, lolPosC) >= per.radius)
		return 0;
	else
		return 1;
	return 0;
}

bool optimizaition(glm::vec3 vectors[8])
{
	for (int i = 0; i < 8; i++)
		if (per.CamFront.x * (vectors[i].x - per.CamPosition.x) + per.CamFront.y * (vectors[i].y - per.CamPosition.y) + per.CamFront.z * (vectors[i].z - per.CamPosition.z) >= 0.0)
			return true;
	return false;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{


	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		
		//PlaySoundA("f.wav", NULL, SND_ASYNC | SND_LOOP);
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_Q:
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_LEFT_CONTROL:
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_W:
			keys[GLFW_KEY_W] = 1;
			break;
		case GLFW_KEY_S:
			keys[GLFW_KEY_S] = 1;
			break;
		case GLFW_KEY_D:
			keys[GLFW_KEY_D] = 1;
			break;
		case GLFW_KEY_A:
			keys[GLFW_KEY_A] = 1;
			break;
		case GLFW_KEY_SPACE:
			keys[GLFW_KEY_SPACE] = 1;
			break;
		default:
			break;
		}
	}
	else
		if (action == GLFW_RELEASE)
			switch (key)
			{
			case GLFW_KEY_W:
				keys[GLFW_KEY_W] = 0;
				break;
			case GLFW_KEY_S:
				keys[GLFW_KEY_S] = 0;
				break;
			case GLFW_KEY_D:
				keys[GLFW_KEY_D] = 0;
				break;
			case GLFW_KEY_A:
				keys[GLFW_KEY_A] = 0;
				break;
			case GLFW_KEY_SPACE:
				keys[GLFW_KEY_SPACE] = 0;
				break;
			default:
				break;
			}
	

		
}
GLfloat xrot, zrot;
glm::vec3 do_movement(Person& per)
{
	glm::vec3 ret = glm::vec3(0.0f);
	bool flag = 0;
	Music = false;
	if (keys[GLFW_KEY_SPACE])
	{
		flag = 1;
	}
	xrot = zrot = 0.0f;
	if (keys[GLFW_KEY_W])
	{
		zrot += 1.0f;
		ret += per.ProcessKeyboard(DFORWARD, deltaTime, flag);
		Music = true;

	}

	if (keys[GLFW_KEY_S])
	{
		zrot -= 1.0f;
		ret += per.ProcessKeyboard(DBACKWARD, deltaTime, flag);
		Music = true;
	}

	if (keys[GLFW_KEY_A])
	{
		xrot += 1.0f;
		ret += per.ProcessKeyboard(DLEFT, deltaTime, flag);
		Music = true;
	}

	if (keys[GLFW_KEY_D])
	{
		xrot -= 1.0f;
		ret += per.ProcessKeyboard(DRIGHT, deltaTime, flag);
		Music = true;
	}


	
	return ret;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	
	if (firstMouse) 
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; 
	lastX = xpos;
	lastY = ypos;
	per.ProcessMouseMovement(xoffset, yoffset);
	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{}

void makeVAOVBO(GLuint &VBO, GLuint &VAO, GLfloat vertices[], int size, int param[][3], int count, bool flagV)
{
	if (flagV) glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	if (flagV)
	{
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	for (int i = 0; i < count; i++)
	{
		glVertexAttribPointer(i, param[i][0], GL_FLOAT, GL_FALSE, sizeof(GLfloat) * param[i][1], (GLvoid*)(param[i][2] * sizeof(GLfloat)));
		glEnableVertexAttribArray(i);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int main()
{
	//mciSendString("play C:\\Users\\User\\Desktop\\OpenGLRoot\\Yoda\\YodaLondon Symphony Orchestra - Cantina Band.mp3 wait" , NULL, 0, NULL);
/*	char nyTb[] = "play C:\\London Symphony Orchestra - Cantina Band.mp3 wait";
	PlaySound(nyTb, NULL, SND_FILENAME);
	mciSendString(nyTb, NULL, 0, NULL);*/
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800,600, "lab4", /*glfwGetPrimaryMonitor()*/nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW" << endl;
		glfwTerminate();
		return -1;
	}
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//AudioDevicePtr device = OpenDevice();
	
	per.LoadModel("rock/car.fbx", 2.1, 2.1, 0.1, 3.14f, 2.5f, 13.0f);
	per.shaderProgram.ShaderLoad("shader.vs", "fragmentShaderSource.txt");
	per.EditModelMatrix(glm::vec3(10.0,0.0,-0.0), glm::vec3(0.005, 0.005, 0.005), xrot, zrot, deltaTime);
	

	AudioDevicePtr device = OpenDevice(); //Для начала нужно открыть AudioDevice 
	cout << "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT" << endl;
	cout << device << endl;
	OutputStreamPtr sound = OpenSound(device, "f.wav", false);

	cout <<sound<< endl;
	OutputStreamPtr ball = OpenSound(device, "ball.wav", false);
	cout << "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT" << endl;
	OutputStreamPtr gimn = OpenSound(device, "gimn.wav", false);
	cout << "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT" << endl;
	sound->setRepeat(1);
	cout << "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN" << endl;
	gimn->setRepeat(1);
	sound->setVolume(0.4f);
	gimn->setVolume(0.2f);
	cout << "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT" << endl;
	
	/*
	Mod Mol("vertexShaderSource.txt", "fragmentShaderSource.txt");
	Mol.LoadModel("Star/star.obj");
	Mol.EditModelMatrix(glm::vec3(16.8f, 0.2, -1.15983f), glm::vec3(1.0, 1.0, 1.0), glm::radians(-83.0f));
	Mol.EditHitBoxMatrix(glm::vec3(2.0, 2.0, 0.5));
	*/
	
	
	
	
	
	
	
	/*
	ModAnim focus("shader.vs", "fragmentShaderSource.txt");
	focus.LoadModel("Robot/robot1.fbx");
	focus.EditModelMatrix(glm::vec3(0.0f), glm::vec3(0.01, 0.01, 0.01), 0.0f);
	focus.EditHitBoxMatrix(glm::vec3(1.0, 2.0, 1.0));
	focus.AnimNow(0);
	*/



	ModAnim DeathStar("shader.vs", "fragmentShaderSource.txt");
	DeathStar.LoadModel("ball/ball.fbx");
	DeathStar.EditModelMatrix(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.01, 0.01, 0.01), 0.0f);
	DeathStar.EditHitBoxMatrix(glm::vec3(1.5, 1.5, 1.5), glm::vec3(0.0,0.0,0.0));

	ModAnim Ship("shader.vs", "fragmentShaderSource.txt");
	Ship.LoadModel("Ship/Ship.fbx");
	Ship.EditModelMatrix(glm::vec3(-40.0f, 10.0f, -30.0f), glm::vec3(0.1, 0.1, 0.1), -80.0f);
	Ship.EditHitBoxMatrix(glm::vec3(1.5, 1.5, 1.5), glm::vec3(0.0, 0.0, 0.0));
	

	Mod Foot("vertexShaderSource.txt", "fragmentShaderSource.txt");
	Foot.LoadModel("Foot/foot.obj");
	Foot.EditModelMatrix(glm::vec3(16.7f, -5.0, -1.8f), glm::vec3(1.0, 1.0, 1.0), glm::radians(0.0f));
	Foot.EditHitBoxMatrix(glm::vec3(2.0, 2.0, 0.5));
	
	Mod Car("vertexShaderSource.txt", "fragmentShaderSource.txt");
	Car.LoadModel("rock/car1.obj");
	Car.EditModelMatrix(glm::vec3(40.0, 0.0, -8.0f), glm::vec3(0.8, 0.8, 0.8), glm::radians(-50.0f));
	Car.EditHitBoxMatrix(glm::vec3(2.0, 2.0, 0.5));

	Mod Bill("vertexShaderSource.txt", "fragmentShaderSource.txt");
	Bill.LoadModel("Bill/cgimodels billboard.obj");
	Bill.EditModelMatrix(glm::vec3(42.0, -1.1, 30.0f), glm::vec3(2.5, 2.5, 2.5), glm::radians(90.0f));
	Bill.EditHitBoxMatrix(glm::vec3(2.0, 2.0, 0.5));

	Mod Car1("vertexShaderSource.txt", "fragmentShaderSource.txt");
	Car1.LoadModel("rock/car2.obj");
	Car1.EditModelMatrix(glm::vec3(40.0, 0.0, 10.0f), glm::vec3(0.8, 0.8, 0.8), glm::radians(-50.0f));
	Car1.EditHitBoxMatrix(glm::vec3(2.0, 2.0, 0.5));

	GLfloat currentFrame;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	GLfloat rot;
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	GLfloat m_startTime = (GLfloat)glfwGetTime();
	LightUnit light;
	light.position = lightPos;
	light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	light.diffuse = glm::vec3(0.98f, 0.361f, 0.094f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ret;
	glm::vec3 tempPerPos;
	bool fleg;
	glm::vec3 tempret;
	projection = glm::mat4(1.0f);
	projection = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);




	float skyboxVertices[] = {
		// positions          
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	Shader Sky_Box("shaderSky.vs", "shaderSky.frag");
	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<std::string> faces
	{
		"sb_mars/front.tga",
		"sb_mars/back.tga",
		"sb_mars/top.tga",
		"sb_mars/bottom.tga",
		"sb_mars/right.tga",
		"sb_mars/left.tga",
	};



	Texture Sky_Tex(GL_TEXTURE_CUBE_MAP);
	Sky_Tex.Load_CubeMap(faces);

	Sky_Box.Use();
	Sky_Box.setInt("skybox", 0);
	

	float distance = 0.0f;
	float speed = 10.0f;
	glm::vec3 front_ball;
	glm::vec3 Pos = DeathStar.Position;

	float Pi = 0.0f;
	float high = 5.0f;
	int k = 0;
	gimn->play();
	while (!glfwWindowShouldClose(window))
	{
		//cout << deltaTime<<endl;
		if (Music == true && !Music_start)
		{
			Music_start = true;
			sound->play();
			//PlaySoundA("f.wav", NULL, SND_ASYNC | SND_LOOP);
		}
		if (!Music)
		{
			sound->stop();
			Music_start = false;
		}


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		ret = glm::vec3(0.0f);
		view = glm::mat4(1.0f);
		tempPerPos = per.Position;
		fleg = 0;
		glfwPollEvents();
		ret = do_movement(per);
	//	cout << "Position " << per.Position.x << " " << per.Position.y << " " << per.Position.z << endl;

		per.AnimNow(xrot, zrot, currentFrame);
		per.EditModelMatrix(per.Position, glm::vec3(0.005, 0.005, 0.005), xrot, zrot, deltaTime);
		DeathStar.AnimNow(1);
		
		if (Ball && distance<5.0f)
		{
			k++;
		//	cout << "k " << k << endl;
			Pos += front_ball*4.0f * 0.05f;
			Pos.y = abs(3*sin(Pi)) - 1.0f;
			//cout << "Pi " << Pi << endl;
			Pi += 0.0314f;
			distance += 0.1f;
		//	cout << "Ball go "<<distance << endl;
			DeathStar.EditModelMatrix(Pos, glm::vec3(0.01, 0.01, 0.01), 0.0f);
			DeathStar.EditHitBoxMatrix(glm::vec3(1.5, 1.5, 1.5), glm::vec3(0.0, 0.0, 0.0));
		//	cout << "x = " << DeathStar.Position.x << " y = " << DeathStar.Position.y << " z = " << DeathStar.Position.z << endl;
		}
		else
			if (Ball && distance < 10.0f)
			{
			//	cout << "Speed " << speed << endl;
			//	cout << "Pi 2 " << Pi << endl;
				Pos += front_ball * speed * 0.05f;

				Pos.y = abs(3 * sin(Pi)) - 1.0f;
				Pi += 0.0314f;

				speed -= 0.03;
				distance += 0.1f;
				DeathStar.EditModelMatrix(Pos, glm::vec3(0.01, 0.01, 0.01), 0.0f);
				DeathStar.EditHitBoxMatrix(glm::vec3(1.5, 1.5, 1.5), glm::vec3(0.0, 0.0, 0.0));
			}
			else
				if (Ball && distance < 20.0f)
				{
				//	cout << "Pi 3 " << Pos.y << "High "<< high<< endl;
					Pos += front_ball * speed * 0.05f;
					Pi += 0.0157;
					Pos.y = high * abs(sin(Pi)) - 1.0f;
					high -= 0.03f;
					speed -= 0.03;
					distance += 0.1f;
					DeathStar.EditModelMatrix(Pos, glm::vec3(0.01, 0.01, 0.01), 0.0f);
					DeathStar.EditHitBoxMatrix(glm::vec3(1.5, 1.5, 1.5), glm::vec3(0.0, 0.0, 0.0));
				}
				else
				{
					DeathStar.AnimNow(0);
					Pos.y=-1.0f;
					Pi = 0.0f;
				//	cout << "PIIIIIIIII " << Pos.y << endl;
					Ball = false;
					distance = 0.0f;
					speed = 4.0f;
					high = 3.0f;
					DeathStar.EditModelMatrix(Pos, glm::vec3(0.01, 0.01, 0.01), 0.0f);
					DeathStar.EditHitBoxMatrix(glm::vec3(1.5, 1.5, 1.5), glm::vec3(0.0, 0.0, 0.0));
				}

		if (!fleg)
		{
			if (compare(per, DeathStar))
			{
				ball->play();
				fleg = 1;
				//cout << "EZIII1" << endl;
				Ball = true;
				front_ball = per.Front;
			}
		}
		
		if (fleg)
		{
			fleg = 0;
			tempret = ret;
			tempret.x = 0;
			per.Position = tempPerPos + tempret;
			if (!fleg)
			{
				if (compare(per, DeathStar))
				{
					fleg = 1;
				//	cout << "EZIII2" << endl;
					Ball = true;
					front_ball = per.Front;
				}
				
			}
			if (fleg)
			{
				fleg = 0;
				tempret = ret;
				tempret.z = 0;
				per.Position = tempPerPos + tempret;
				if (!fleg)
				{
					if (compare(per, DeathStar))
					{
						fleg = 1;
						//cout << "EZIII3" << endl;
					}
				}
				if (fleg)
				{
					per.EditModelMatrix(tempPerPos, glm::vec3(0.005, 0.005, 0.005), xrot, zrot, deltaTime);
				//	cout << "EZIII4" << endl;
					Ball = true;
					front_ball = per.Front;
				}
			}

		}
		

		view = per.GetViewMatrix();
		per.LoadShaderUniform(view, projection, per.CamPosition, light, currentFrame);
		per.Draw();

		Ship.AnimNow(1);
		
		DeathStar.LoadShaderUniform(view, projection, per.CamPosition, light, currentFrame);
		DeathStar.Draw(view, projection, 0);

		

		Ship.LoadShaderUniform(view, projection, per.CamPosition, light, currentFrame);
		Ship.Draw(view, projection, 0);
		
		Foot.LoadShaderUniform(view, projection, per.CamPosition, light, currentFrame);
		Foot.Draw(view, projection, 0);

		Car.LoadShaderUniform(view, projection, per.CamPosition, light, currentFrame);
		Car.Draw(view, projection, 0);

		Car1.LoadShaderUniform(view, projection, per.CamPosition, light, currentFrame);
		Car1.Draw(view, projection, 0);

		Bill.LoadShaderUniform(view, projection, per.CamPosition, light, currentFrame);
		Bill.Draw(view, projection, 0);
		/*if (optimizaition(focus.vectors))
		focus.LoadShaderUniform(view, projection, per.CamPosition, light, currentFrame);
		focus.Draw(view, projection, 0);
		}
		*/
		



		glDepthFunc(GL_LEQUAL);

		Sky_Box.Use();
		
		view = glm::mat4(glm::mat3(per.GetViewMatrix()));
		Sky_Box.setMat4("view", view);
		Sky_Box.setMat4("projection", projection);

		glBindVertexArray(skyboxVAO);
		Sky_Tex.Bind(GL_TEXTURE0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		/*Mol.LoadShaderUniform(view, projection, per.CamPosition, light, currentFrame);
		Mol.Draw(view, projection,0);*/

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
