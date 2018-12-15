#include "Camera.h"

float lastX = 400, lastY = 300;
float pitch = 0, yaw = 0;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
	{
		firstMouse = true;
		return;
	}

	if (firstMouse) // this bool variable is initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}

Camera::Camera(GLFWwindow* window)
{
	glfwSetCursorPosCallback(window, mouse_callback);
}

void Camera::processInput(float deltaTime)
{
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);

	float cameraSpeed = 4.5f*deltaTime; // adjust accordingly
	if (Input::GetButtonState(W) == DOWN)
		cameraPos += cameraSpeed * cameraFront;
	if (Input::GetButtonState(E) == DOWN)
		cameraPos += cameraSpeed * cameraUp;
	if (Input::GetButtonState(Q) == DOWN)
		cameraPos += cameraSpeed * -cameraUp;
	if (Input::GetButtonState(S) == DOWN)
		cameraPos -= cameraSpeed * cameraFront;
	if (Input::GetButtonState(A) == DOWN)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (Input::GetButtonState(D) == DOWN)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

glm::vec3 Camera::GetCamFront()
{
	return cameraFront;
}

glm::vec3 Camera::GetCamPos()
{
	return cameraPos;
}

glm::mat4 Camera::GetProjection()
{
	//viewport variables
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	float width = m_viewport[2];
	float height = m_viewport[3];
	float dim = width / (height == 0 ? 600 : height);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(fieldOfView), dim, 0.1f, 100.0f);
	return projection;
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return view;
}

void Camera::Update(float deltaTime)
{
	processInput(deltaTime);
}

