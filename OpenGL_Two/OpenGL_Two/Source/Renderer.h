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

class Renderer
{
public:
	Renderer(GLFWwindow* window);
	unsigned int VAO;
	void StartRendering();
	void Render(GLFWwindow*, float deltaTime, Camera* mainCam, 
		Drawable* objectToDraw, Material* mat, glm::mat4 objectTransform, std::vector<class Light*> lights);
	void FinishRendering(GLFWwindow*);
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
};

