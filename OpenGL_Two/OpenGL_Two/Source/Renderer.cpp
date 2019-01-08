#include "Renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Input.h"

unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;
bool screenSizeChanged = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	screenSizeChanged = true;
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

Renderer::Renderer(GLFWwindow* window)
{
	//set screen resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//framebuffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//color attachment texture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	//render buffer
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); 

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Debug::Log("framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//setup the quad object
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//screen shader
	screenShader = Shader(
		"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Shaders/screenShader.vs",
		"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Shaders/screenShader.fs");

	//skybox
	skybox.loadCubemap(cubemapPaths);
}

void Renderer::StartRendering(Camera* mainCamera, std::vector<class Light*>* lights)
{
	if (Input::GetButtonState(NUM0) == BUTTONSTATE::PRESSED)
	{
		postProcessing = !postProcessing;

		if (!postProcessing)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (screenSizeChanged)
	{
		//color attachment texture resize
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		//render buffer resize
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		screenSizeChanged = false;
	}

	if (postProcessing)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST); //depth testing is disabled for rendering screen-space quad
	}

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//uniform buffer matrices
	SetMatrixUniformBufferData(mainCamera);

	//Uniform light buffer
	SetLightUniformBufferData(lights);
}

void Renderer::Render(GLFWwindow* window,float deltaTime, Camera* mainCam, Drawable* objectToDraw, Material* mat, glm::mat4 objectTransform, std::vector<Light*> lights)
{
	//render identical objects in the same drawcall later on the line
	//here we could form a mega buffer for all the identical objects
	objectToDraw->Draw(mainCam, mat, objectTransform, lights);
}

void Renderer::FinishRendering(GLFWwindow* window, Camera* mainCam)
{
	//draw skybox
	skybox.DrawCubemap(mainCam);

	if (postProcessing)
	{
		//bind back to default framebuffer and draw a quad plane with the framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	/* Swap front and back buffers */
	glfwSwapBuffers(window);
}

void Renderer::SetMatrixUniformBufferData(Camera* mainCamera)
{
	// set the uniform buffers so that we dont have to set values each time object is drawn each frame, 
	// if the value does not change. 
	{
		//individual shader bindings
		//currently in the Game object

		//actual buffer setup
		unsigned int uboMatrices;
		glGenBuffers(1, &uboMatrices);

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

		glm::mat4 projection = mainCamera->GetProjection();
		glm::mat4 view = mainCamera->GetViewMatrix();

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void Renderer::SetLightUniformBufferData(std::vector<Light*>* lights)
{
	DirectionalLightVariables dirVar;
	int directionalLights = 0;

	std::vector<PointLightVariables> pointLightData;
	int pointLights = 0;

	for (Light* l : *lights)
	{
		switch (l->type)
		{
		case Directional:
			//DIRECTIONAL LIGHT
			dirVar.diffuse = glm::vec4(l->diffuse, 1.0f);
			dirVar.specular = glm::vec4(l->specular, 1.0f);
			dirVar.direction = glm::vec4(l->direction, 1.0f);

			directionalLights++;
			break;
		case Point:
			PointLightVariables p;

			p.position = glm::vec4(l->position, 1.0f);

			p.constant = l->attenuation.constant;
			p.linear = l->attenuation.linear;
			p.quadratic = l->attenuation.quadratic;

			p.diffuse = glm::vec4(l->diffuse, 1.0f);
			p.specular = glm::vec4(l->specular, 1.0f);

			pointLightData.push_back(p);
			pointLights++;
			break;
		case Spotlight:
			break;
		case Ambient:
			break;
		default:
			break;
		}
	}
	#pragma region directional light

	//actual light buffer setup
	unsigned int uboDirLights;
	glGenBuffers(1, &uboDirLights);

	glBindBuffer(GL_UNIFORM_BUFFER, uboDirLights);
	//how big is it all in all?
	glBufferData(GL_UNIFORM_BUFFER, sizeof(DirectionalLightVariables), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboDirLights, 0, sizeof(DirectionalLightVariables));
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboDirLights);

	//glBindBuffer(GL_UNIFORM_BUFFER, uboDirLights);
	//variable sizes and offsets
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(DirectionalLightVariables), &dirVar);
	//var 2
	//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	#pragma endregion

	#pragma region point lights

	unsigned int uboPointLights;
	glGenBuffers(1, &uboPointLights);
	glBindBuffer(GL_UNIFORM_BUFFER, uboPointLights);

	glBufferData(GL_UNIFORM_BUFFER, 64 * sizeof(PointLightVariables), NULL, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboPointLights);
	//glBindBuffer(GL_UNIFORM_BUFFER, uboPointLights);

	for (int i = 0; i < pointLights; i++)
	{
		//how big is it all in all?

		//glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboDirLights, 0, sizeof(DirectionalLightVariables));

		glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(PointLightVariables), sizeof(PointLightVariables), &pointLightData[i]);
	}

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	#pragma endregion
}
