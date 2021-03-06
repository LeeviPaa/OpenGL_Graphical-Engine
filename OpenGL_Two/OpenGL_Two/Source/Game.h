#pragma once
#include "Drawable.h"
#include "Renderer.h"
#include "Input.h"
#include "Meshes/Cube.h"
#include "Camera.h"
#include "Materials/Material.h"
#include "Lighting/Light.h"
#include "Materials/Material_A.h"
#include "Materials/Material_Light.h"
#include "Materials/Material_Chrome.h"
#include "Materials/Material_Ice.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp> 
#include <glm/ext.hpp> 
#include "Meshes/Model.h"

class Game
{
private:
	Camera * mainCamera;
	GLFWwindow * renderWindow;
	Renderer* mainRender;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	Material_A materialLit;
	Material_Chrome materialChrome;
	Material_Ice materialIce;
	Material_Light materialLight;
	Cube cubes;

	Model* floor;
	Model* nanosuit;
	//nanosuit has its own materials inside for now
	//TODO: create a renderer object that has ref to both model/mesh and the materials

	glm::vec3 pointLightPositions[4] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, 3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	Light* pointLights[4];
	Light* sceneLight;

	//grass mesh
	Mesh* grass;
	Material_A grassMat;

	std::vector<Light*> sceneLights;
	//std::vector<Light*> lights that affect the model we are about to draw

public:
	Game(GLFWwindow*);
	void Update();
	~Game();
};

