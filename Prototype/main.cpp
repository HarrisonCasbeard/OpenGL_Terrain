//Std Libraries
#include <stdio.h>
#include <stdlib.h>
#include <memory>

//OpenGL, GLEW, GLM
#include <glew\glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

//My Classes
#include "Camera.h"
#include "Command.h"
#include "FrameBuffer.h"
#include "GameObject3D.h"
#include "GfxEngine.h"
#include "Input.h"
#include "Light.h"
#include "ModelManager.h"
#include "PhysicsManager.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "ShadowMap.h"
#include "Skybox.h"
#include "Terrain.h"
#include "TextManager.h"
#include "Timer.h"
#include "Water.h"
#include "WaterFrameBuffer.h"

const int SCREEN_WIDTH = 1240;
const int SCREEN_HEIGHT = 930;

void framesizeBufferCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void initOpenGL(GLFWwindow* window, Camera& cam)
{
	//----- Create a Window using GLFW -----
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					//Sets the necessary version of OpenGL (the tutorial is using 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//Tells OpenGL what subset of functions we want (the core profile)

	if (!window)
	{
		printf("FAILED TO CREATE WINDOW!!");
		glfwTerminate();
		__debugbreak();
	}
	glfwMakeContextCurrent(window);

	//Init glew
	if (glewInit())
	{
		printf("FAILED TO INITIALISE GLEW!!");
		glfwTerminate();
		__debugbreak();
	}

	//----- Initialise Viewport -----
	int height, width;
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framesizeBufferCallback);
}

int main()
{
	//------------------------------------
	//-     Initialise GLFW & OpenGL     -
	//------------------------------------
	if (!glfwInit())
	{
		printf("FAILED TO INITIALISE GLFW!!");
		__debugbreak();
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Prototype", NULL, NULL);

	Timer timer;
	Camera camera;
	camera.initialise({ -256.0f, 0.0f, 256.0f }, { 0, 1, 0 }, 0, 270, 0);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	initOpenGL(window, camera);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 1500.0f);

	//------------------
	//-   Load Input   -
	//------------------
	Input input;
	input.initialise(camera, SCREEN_WIDTH, SCREEN_HEIGHT);

	//-----------------
	//- Load Managers -
	//-----------------
	PhysicsManager* physicsManager = new PhysicsManager();

	ShaderManager shaderManager;
	shaderManager.initialiseShaders(SCREEN_WIDTH, SCREEN_HEIGHT, projection);

	ModelManager modelManager;

	TextManager textManager;
	textManager.initialise((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	GfxEngine gfxEngine;
	//-----------------------------------

	//------------------------------------
	//-     Frame Buffer Initialises     -
	//------------------------------------
	FrameBuffer frameBuffer = FrameBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	//frameBuffer.enabled(false);

	WaterFrameBuffer waterReflectionBuffer, waterRefractionBuffer;
	waterReflectionBuffer.addColourTextureAttachment(SCREEN_WIDTH, SCREEN_HEIGHT);
	waterReflectionBuffer.addDepthTextureAttachment(SCREEN_WIDTH, SCREEN_HEIGHT);

	waterRefractionBuffer.addColourTextureAttachment(SCREEN_WIDTH, SCREEN_HEIGHT);
	waterRefractionBuffer.addDepthBufferAttachment(SCREEN_WIDTH, SCREEN_HEIGHT);

	ShadowMap shadowMap;
	shadowMap.initialise();
	//-----------------------------------

	//---------------------------
	//- Create All Game Objects -
	//---------------------------
	Light light({ 0.0f, 100.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

	Skybox skybox;
	skybox.initialise({ "Assets/Textures/Skybox/SeaMountains/right.jpg",
						"Assets/Textures/Skybox/SeaMountains/left.jpg",
						"Assets/Textures/Skybox/SeaMountains/top.jpg",
						"Assets/Textures/Skybox/SeaMountains/bottom.jpg",
						"Assets/Textures/Skybox/SeaMountains/front.jpg",
						"Assets/Textures/Skybox/SeaMountains/back.jpg" },
						*shaderManager.getShader("skybox"));

	Terrain terrain(*shaderManager.getShader("terrain"), 75.0f, 2.0f);
	terrain.setShadowMap(shadowMap.textureID());
	terrain.loadHeightMap("Assets/Heightmaps/terrain0-8bbp-257x257.raw", 8, 257, 257);
	terrain.loadTexture("Assets/Textures/grass.png", 0);
	terrain.loadTexture("Assets/Textures/rock.png", 1);
	terrain.loadTexture("Assets/Textures/snow.jpg", 2);

	GameObject3D lightBall(Model(modelManager.getModel("meteor")), light.position(), *shaderManager.getShader("basic"));

	Water waterTile(*shaderManager.getShader("water"), { 0.0f, 100.0f, 0.0f }, glm::vec3(100.0f));
	//--------------------------------

	//-------------------------
	//- Create Rendering List -
	//-------------------------
	gfxEngine.addGfx("terrain", &terrain);
	//gfxEngine.addGfx("waterTile1", &waterTile);
	gfxEngine.addGfx("lightBall", &lightBall);

	//Wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	std::vector<Command*> commands;
	GameObject3D* placeholder = nullptr;
	//----- Game Loop -----
	while (!glfwWindowShouldClose(window))
	{
		//----- Input -----
		input.update(window, timer.getDeltaTime(), commands);
		for (unsigned int c = 0; c < commands.size(); c++)
		{
			if (!commands[c]->execute(*placeholder, timer.getDeltaTime()))
			{
				glfwSetWindowShouldClose(window, true);
			}
		}

		//Quick hack to test lighting!
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			light.position({ 20.0f * timer.getDeltaTime(), 0.0f, 0.0f });
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			light.position({ -20.0f * timer.getDeltaTime(), 0.0f, 0.0f });
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			light.position({ 0.0f, 20.0f * timer.getDeltaTime(), 0.0f });
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			light.position({ 0.0f, -20.0f * timer.getDeltaTime(), 0.0f });

		//----- Update ------
		timer.update();
		shaderManager.updateShaders(camera.getViewMatrix(), camera.getPosition(), light);

		if(physicsManager->update(timer.getDeltaTime()))
			glfwSetWindowShouldClose(window, true);

		lightBall.position(light.position());
	
		//----- Shadow Render -----
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		shadowMap.bind();
		gfxEngine.renderAll();
		shadowMap.unbind({ SCREEN_WIDTH, SCREEN_HEIGHT });

		//----- Normal Rendering -----
		//Everything below will be drawn onto the frame buffer, if enabled
		if (frameBuffer.enabled())
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.frameBufferId());

		//Reset the viewport
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		gfxEngine.renderAll();
		skybox.render(camera.getViewMatrix(), projection);

		if (frameBuffer.enabled())
		{
			shaderManager.getShader("framebuffer")->activate();
			frameBuffer.draw();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete physicsManager;
	glfwTerminate();
	return 0;
}