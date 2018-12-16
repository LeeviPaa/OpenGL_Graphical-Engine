#include "Game.h"
#include "Renderer.h"
#include <iostream>

bool gate = true;
static void input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		return;
	}

	if (key == GLFW_KEY_KP_0 && action == GLFW_PRESS)
	{
		if (gate)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		gate = !gate;
	}

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		if(gate)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		gate = !gate;
	}
}

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

Game::Game(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	renderWindow = window;
	mainRender = new Renderer();
	mainCamera = new Camera(window);

	//bind some keys
	glfwSetKeyCallback(window, input_callback);

	//init the cube buffers
	cubes.Initialize();
	//load one material
	materialLit.Initialize();

	materialLight.Initialize();

	sceneLight = new Light(LightType::Directional,
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f),
		glm::vec3(-0.4f, -1.0f, -0.4f));
	//add it to the list
	sceneLights.push_back(sceneLight);

	for (int i = 0; i < 4; i++)
	{
		pointLights[i] = new Light(
			LightType::Point,
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			pointLightPositions[i],
			glm::vec3(1.0f));

		sceneLights.push_back(pointLights[i]);
	}

	nanosuit = new Model("G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Models/Nanosuit/nanosuit.obj");
}

void Game::Update()
{
	//calculate the deltatime
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	//check input
	Input::UpdateInputs(renderWindow, deltaTime);

	//update logic
	//foreach updateable in the list -> Updateable->Update();
	mainCamera->Update(deltaTime);

	//render stuff
	mainRender->StartRendering();

	glm::vec3 color = glm::vec3(
		1.3f + (sin(currentFrame) / 2.0f),
		1.3f + (sin(currentFrame + glm::radians(180.0f)) / 2.0f),
		1.3f + (sin(currentFrame + glm::radians(90.0f)) / 2.0f)
	);
	materialLight.lightColor = color;
	for (Light* l : sceneLights)
	{
		if(l->type == LightType::Point)
			l->diffuse = color * 0.3f;
	}
	
	//LIGHTS!!
	for (int i = 0; i < 4; i++)
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, pointLightPositions[i]);
		trans = glm::scale(trans, glm::vec3(0.1f, 0.1f, 0.1f));

		mainRender->Render(renderWindow, deltaTime, mainCamera, sceneLight, &materialLight, trans, sceneLights);
	}

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.0f, -2.0f, -2.0f));
	transform = glm::scale(transform, glm::vec3(0.25f));
	mainRender->Render(renderWindow, deltaTime, mainCamera, nanosuit, &materialLit, transform, sceneLights);

	//this needs to be optimized so that similar objects are rendered in the same draw call
	/*for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, cubePositions[i]);

		mainRender->Render(renderWindow, deltaTime, mainCamera, &cubes, &materialLit, transform, sceneLights);
	}*/

	mainRender->FinishRendering(renderWindow);
}

Game::~Game()
{
	for (int i = 0; i < 4; i++)
	{
		delete pointLights[i];
	}
	delete(sceneLight);
	delete(mainCamera);
	delete(mainRender);
}
