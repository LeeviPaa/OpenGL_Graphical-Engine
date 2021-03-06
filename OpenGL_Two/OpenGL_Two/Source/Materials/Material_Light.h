#pragma once
#include "Material.h"

class Material_Light :
	public Material
{
	//texture thingys
	int width, height, nrChannels;
	unsigned char *data;

public:
	glm::vec3 lightColor = glm::vec3(1.0f);

	void Initialize() override {
		shaderOne = Shader(
			"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Shaders/shader_Lit.vs",
			"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Shaders/shader_Light.fs");
		shaderOne.use();

	}
	void InitDraw(glm::mat4 model, Camera* drawCam, std::vector<Light*> lights) override {
		shaderOne.use();

		float time = glfwGetTime();

		shaderOne.setVec3("objectColor", lightColor);

		//draw matrix variables

		shaderOne.setMat4("model", model);

	}
	Material_Light(){}
	Material_Light(const Material_Light& mat) : Material(mat) {}
};

