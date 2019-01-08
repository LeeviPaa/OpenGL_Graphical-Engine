#pragma once
#include "Material.h"

class Material_Ice :
	public Material
{
public:
	unsigned int cubemapTexture;

	void Initialize() override {
		shaderOne = Shader(
			"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Shaders/shader_chrome.vs",
			"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Shaders/shader_ice.fs");
		shaderOne.use();

		//enable depth testing
		glEnable(GL_DEPTH_TEST);
		//enable blending

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}


	void InitDraw(glm::mat4 model, Camera* drawCam, std::vector<Light*> lights) override {
		shaderOne.use();

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		//draw matrix variables
		shaderOne.setVec3("camPos", drawCam->GetCamPos());

		shaderOne.setMat4("model", model);
	}
	Material_Ice() {}
	Material_Ice(const Material_Ice &mat) : Material(mat) {}
};

