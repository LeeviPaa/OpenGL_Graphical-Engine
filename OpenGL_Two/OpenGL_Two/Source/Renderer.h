#pragma once
#include "Camera.h"
#include "Debug.h"
#include "Input.h"
#include "Drawable.h"
#include "Materials/Material.h"
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Meshes/Mesh.h"
#include "Materials/Cubemap.h"
#include "FileTools.h"

class Renderer
{
public:
	Cubemap skybox;

	Renderer(GLFWwindow* window);
	unsigned int VAO;
	void StartRendering();
	void Render(GLFWwindow*, float deltaTime, Camera* mainCam, 
		Drawable* objectToDraw, Material* mat, glm::mat4 objectTransform, std::vector<class Light*> lights);
	void FinishRendering(GLFWwindow* window, Camera* mainCam);
private:
	float quadVertices[24] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	unsigned int framebuffer, textureColorbuffer, rbo;
	unsigned int quadVAO, quadVBO;
	Shader screenShader;
	bool postProcessing = true;

	std::vector<std::string> cubemapPaths = {
		"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Textures/SkyboxA/cubemap +X.png",
		"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Textures/SkyboxA/cubemap -X.png",
		"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Textures/SkyboxA/cubemap +Y.png",
		"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Textures/SkyboxA/cubemap -Y.png",
		"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Textures/SkyboxA/cubemap +Z.png",
		"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Textures/SkyboxA/cubemap -Z.png" };
};

