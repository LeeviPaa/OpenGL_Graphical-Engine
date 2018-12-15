#pragma once
#include "Material.h"
#include "../stb_image.h"

class Material_B :
	public Material
{
	//texture thingys
	int width, height, nrChannels;
	unsigned char *data;

public:
	unsigned int texture1, texture2;
	void Initialize() override {
		shaderOne = Shader(
			"G:/Projects/OpenGL/MinecraftClone/OpenGL_Two/OpenGL_Two/Source/Shaders/shader_A.vs",
			"G:/Projects/OpenGL/MinecraftClone/OpenGL_Two/OpenGL_Two/Source/Shaders/shader_A.fs");
		shaderOne.use();

		//create the texture
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(true);

		//load the texture data	
		data = stbi_load("G:/Projects/OpenGL/MinecraftClone/OpenGL_Two/OpenGL_Two/Source/Textures/wall.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else Debug::Log("Failed to load texture data");
		stbi_image_free(data);

		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//load the other texture data	
		unsigned char *dataA = stbi_load("G:/Projects/OpenGL/MinecraftClone/OpenGL_Two/OpenGL_Two/Source/Textures/awesomeface.png", &width, &height, &nrChannels, 0);
		if (dataA)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataA);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else Debug::Log("Failed to load texture data");

		stbi_image_free(dataA);

		//we NEED to set this before changing shader properties below!!!
		shaderOne.use();
		//Set the shader properties
		glUniform1i(glGetUniformLocation(shaderOne.ID, "texture1"), 0);
		shaderOne.setInt("texture2", 1);
		shaderOne.setFloat("alpha", 0.2f);

		//enable depth testing
		glEnable(GL_DEPTH_TEST);
	}
	void InitDraw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, Camera* drawCam, std::vector<Light*> lights) override {
		shaderOne.use();

		//draw matrix variables
		shaderOne.setMat4("view", view);

		shaderOne.setMat4("projection", projection);

		shaderOne.setMat4("model", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
	}
	Material_B() {};
};