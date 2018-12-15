#include "Renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Renderer::Renderer()
{
	
}

Renderer::~Renderer()
{

}

void Renderer::StartRendering()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render(GLFWwindow* window,float deltaTime, Camera* mainCam, Drawable* objectToDraw, Material* mat, glm::mat4 objectTransform, std::vector<Light*> lights)
{
	//render identical objects in the same drawcall later on the line
	//here we could form a mega buffer for all the identical objects
	objectToDraw->Draw(mainCam, mat, objectTransform, lights);
}

void Renderer::FinishRendering(GLFWwindow* window)
{
	/* Swap front and back buffers */
	glfwSwapBuffers(window);
}
