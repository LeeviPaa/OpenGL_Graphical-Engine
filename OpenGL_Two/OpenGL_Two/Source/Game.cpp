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
	glm::vec3(5.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-3.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(7.4f, -0.4f, -3.5f),
	glm::vec3(-3.7f,  3.0f, -7.5f),
	glm::vec3(3.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  6.0f, -2.5f),
	glm::vec3(11.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  8.0f, -1.5f)
};

Game::Game(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	renderWindow = window;
	mainRender = new Renderer(window);
	mainCamera = new Camera(window);

	//bind some keys
	glfwSetKeyCallback(window, input_callback);

	//init the cube buffers
	cubes.Initialize();
	//load one material
	materialLit.Initialize();
	materialLit.setMaterialTexture(
		"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Textures/container2.png",
		aiTextureType::aiTextureType_DIFFUSE);
	materialLit.setMaterialTexture(
		"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Textures/container2_specular.png",
		aiTextureType::aiTextureType_SPECULAR);

	materialLight.Initialize();
	materialChrome.Initialize();
	materialIce.Initialize();

	sceneLight = new Light(LightType::Directional,
		glm::vec3(0.25f, 0.2f, 0.22f),
		glm::vec3(0.25f, 0.2f, 0.22f),
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

	//models ------------------------------------
	nanosuit = new Model("G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Models/Nanosuit/nanosuit.obj");
	floor = new Model("G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Models/Floor/floor.obj");

	//grass ------------------------------------
	std::vector<Vertex> vertices;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			Vertex v;
			v.Position = glm::vec3(x, y, 0.0f);
			
			v.TexCoords = glm::vec2(x, glm::abs(y-1));

			v.Normal = glm::vec3(0.0f, 0.0f,1.0f);
			/*v.Bitangent = glm::vec3(0.0f);
			v.Tangent = glm::vec3(0.0f);*/

			vertices.push_back(v);
		}
	}
	std::vector<unsigned int> indices{
		0, 3, 1,
		0, 2, 3
	};

	grass = new Mesh(vertices, indices);
	grassMat.Initialize();
	grassMat.setMaterialTexture(
		"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Textures/blending_transparent_window.png",
		aiTextureType::aiTextureType_DIFFUSE);
	grassMat.GetShader()->setRenderType(Transparent);
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
	mainRender->StartRendering(mainCamera, &sceneLights);

	//NOTE:These bindings seem to work without us specifically having to declare to
	//as is done in the following lines
	#pragma region Uniform buffer material bindings

	//foreach (material that uses view matrices)
	{
		Shader* lit = materialLit.GetShader();
		unsigned int informBlockIndexLit = glGetUniformBlockIndex(lit->ID, "Matrices");
		Shader* chrome = materialChrome.GetShader();
		unsigned int informBlockIndexChrome = glGetUniformBlockIndex(chrome->ID, "Matrices");
		Shader* ice = materialIce.GetShader();
		unsigned int informBlockIndexIce = glGetUniformBlockIndex(ice->ID, "Matrices");
		Shader* light = materialLight.GetShader();
		unsigned int informBlockIndexLight = glGetUniformBlockIndex(light->ID, "Matrices");

		glUniformBlockBinding(lit->ID, informBlockIndexLit, 0);
		glUniformBlockBinding(chrome->ID, informBlockIndexChrome, 0);
		glUniformBlockBinding(ice->ID, informBlockIndexIce, 0);
		glUniformBlockBinding(light->ID, informBlockIndexLight, 0);
	}

	//foreach (lit material)
	{
		Shader* lit = materialLit.GetShader();
		Shader* chrome = materialChrome.GetShader();
		Shader* ice = materialIce.GetShader();
		Shader* light = materialLight.GetShader();

		unsigned int informBlockIndexLit = glGetUniformBlockIndex(lit->ID, "PointLights");
		unsigned int informBlockIndexChrome = glGetUniformBlockIndex(chrome->ID, "PointLights");
		unsigned int informBlockIndexIce = glGetUniformBlockIndex(ice->ID, "PointLights");
		unsigned int informBlockIndexLight = glGetUniformBlockIndex(light->ID, "PointLights");

		glUniformBlockBinding(lit->ID, informBlockIndexLit, 1);
		glUniformBlockBinding(chrome->ID, informBlockIndexChrome, 1);
		glUniformBlockBinding(ice->ID, informBlockIndexIce, 1);
		glUniformBlockBinding(light->ID, informBlockIndexLight, 1);

		informBlockIndexLit = glGetUniformBlockIndex(lit->ID, "DirectionalLight");
		informBlockIndexChrome = glGetUniformBlockIndex(chrome->ID, "DirectionalLight");
		informBlockIndexIce = glGetUniformBlockIndex(ice->ID, "DirectionalLight");
		informBlockIndexLight = glGetUniformBlockIndex(light->ID, "DirectionalLight");

		glUniformBlockBinding(lit->ID, informBlockIndexLit, 2);
		glUniformBlockBinding(chrome->ID, informBlockIndexChrome, 2);
		glUniformBlockBinding(ice->ID, informBlockIndexIce, 2);
		glUniformBlockBinding(light->ID, informBlockIndexLight, 2);
	}
	#pragma endregion

	// ----------------------------------------------------------------------------
	// RENDER ORDER
	//
	//	1. Draw all opaque objects first.
	//	2. Sort all the transparent objects.
	//	3. Draw all the transparent objects in sorted order.
	//
	// NOTE: default lit material (Material_A) is opaque by default
	// ----------------------------------------------------------------------------

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
	
	//render the light objects--------------------------------------
	for (int i = 0; i < 4; i++)
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, pointLightPositions[i]);
		trans = glm::scale(trans, glm::vec3(0.1f, 0.1f, 0.1f));

		mainRender->Render(renderWindow, deltaTime, mainCamera, sceneLight, &materialLight, trans, sceneLights);
	}

	// render the nanosuit--------------------------------------
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.0f, -3.0f, -2.0f));
	transform = glm::scale(transform, glm::vec3(0.25f));
	//for now the model class uses the materials provided within, so no need to provide a material
	for (int i = 0; i < 1; i++) //stress test
	{
		mainRender->Render(renderWindow, deltaTime, mainCamera, nanosuit, nullptr, transform, sceneLights);
	}

	// render chrome nanosuit--------------------------------------
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(2.5f, -3.0f, -2.0f));
	transform = glm::scale(transform, glm::vec3(0.25f));
	for (int i = 0; i < 1; i++) //stress test
	{
		//this is ghetto af
		materialChrome.cubemapTexture = mainRender->skybox.cubemapTexture;

		mainRender->Render(renderWindow, deltaTime, mainCamera, nanosuit, &materialChrome, transform, sceneLights);
	}

	// render glass nanosuit--------------------------------------
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(-2.5f, -3.0f, -2.0f));
	transform = glm::scale(transform, glm::vec3(0.25f));
	for (int i = 0; i < 1; i++) //stress test
	{
		//this is ghetto af
		materialIce.cubemapTexture = mainRender->skybox.cubemapTexture;

		mainRender->Render(renderWindow, deltaTime, mainCamera, nanosuit, &materialIce, transform, sceneLights);
	}

	//render the floor--------------------------------------
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.0f, -3.0f, -2.0f));
	transform = glm::scale(transform, glm::vec3(2.0f, 1.0f, 2.0f));

	for (Material_A &var : floor->materials)
	{
		var.GetShader()->use();
		var.GetShader()->setFloat("uvScale", 5);
	}
	mainRender->Render(renderWindow, deltaTime, mainCamera, floor, nullptr, transform, sceneLights);

	//render the boxes--------------------------------------
	for (unsigned int i = 0; i < 1; i++)
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, cubePositions[i]);

		mainRender->Render(renderWindow, deltaTime, mainCamera, &cubes, &materialChrome, transform, sceneLights);
	}

	// ----------------------------------------------------------------------------
	// RENDER TRANSPARENT OBJECTS SORTED BY DISTANCE HERE!!! 
	// ----------------------------------------------------------------------------
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.0f, -2.0f, -0));

		mainRender->Render(renderWindow, deltaTime, mainCamera, grass, &grassMat, trans, sceneLights);
	}

	mainRender->FinishRendering(renderWindow, mainCamera);
}

Game::~Game()
{
	for (int i = 0; i < 4; i++)
	{
		delete pointLights[i];
	}
	delete(grass);
	delete(nanosuit);
	delete(sceneLight);
	delete(mainCamera);
	delete(mainRender);
}
