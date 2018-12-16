#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Drawable.h"
#include "Game.h"
#include "Input.h"
#include "Debug.h"
#include <iostream>
#include "FileTools.h"

//REMOVE THESE, USED TO CHECK IF THIS SHIT COMPILES WITH THEM
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

std::string FileTools::programPath = "";
int main(int argc, char* argv[])
{
	//set the root folder of the program
	FileTools::SetPath(argv[0]);

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//sets the error callback
	glfwSetErrorCallback(error_callback);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Minecraft clone", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		system("pause");
		return -1;
	}


	//set screen resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Game* gameHandler = new Game(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		gameHandler->Update();

		/* Poll for and process events */
		glfwPollEvents();
	}

	//DONT REMOVE THIS 
	delete(gameHandler);

	glfwTerminate();
	return 0;
}