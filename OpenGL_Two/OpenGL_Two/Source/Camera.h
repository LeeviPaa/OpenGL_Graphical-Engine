#pragma once
#include "Transform.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input.h"

class Camera: public Transform
{
protected:
	float cameraSpeed = 0.05f;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	void processInput(float deltaTime);
public:
	glm::vec3 GetCamFront();
	glm::vec3 GetCamPos();
	float fieldOfView = 60.0f;
	Camera(GLFWwindow* window);
	glm::mat4 GetProjection();
	glm::mat4 GetViewMatrix();
	void Update(float deltaTime);
};

