#pragma once
#include "Material.h"
#include "../stb_image.h"

class Material_A :
	public Material
{
	//texture thingys
	int width, height, nrChannels;
	unsigned char *data;

public:
	unsigned int diffuseMap, specularMap, emissionMap;

	bool LoadTexture(unsigned int* textureID, bool hasAlpha, bool flipTexture, const char* texturePath)
	{
		bool success = false;
		//create the texture
		glGenTextures(1, textureID);
		glBindTexture(GL_TEXTURE_2D, *textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if(flipTexture) stbi_set_flip_vertically_on_load(true);

		//load the texture data	
		data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
		if (data)
		{
			if(hasAlpha) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			glGenerateMipmap(GL_TEXTURE_2D);
			success = true;
		}
		else Debug::Log("Failed to load texture data");
		stbi_image_free(data);

		return success;
	}

	void Initialize() override {
		shaderOne = Shader(
			"G:/Projects/OpenGL/MinecraftClone/OpenGL_Two/OpenGL_Two/Source/Shaders/shader_Lit.vs",
			"G:/Projects/OpenGL/MinecraftClone/OpenGL_Two/OpenGL_Two/Source/Shaders/shader_Lit.fs");
		shaderOne.use();

		//create the texture
		if (!LoadTexture(&diffuseMap, true, true, "G:/Projects/OpenGL/MinecraftClone/OpenGL_Two/OpenGL_Two/Source/Textures/container2.png"))
		{
			Debug::Log("Failed to load diffuse");
		}

		if(!LoadTexture(&specularMap, true, true, "G:/Projects/OpenGL/MinecraftClone/OpenGL_Two/OpenGL_Two/Source/Textures/container2_specular.png"))
		{
			Debug::Log("Failed to load specular");
		}
		if(!LoadTexture(&emissionMap, false, true, "G:/Projects/OpenGL/MinecraftClone/OpenGL_Two/OpenGL_Two/Source/Textures/matrix.jpg"))
		{
			Debug::Log("Failed to load emission");
		}
		

		//we NEED to set this before changing shader properties below!!!
		shaderOne.use();
		//Set the shader properties
		shaderOne.setInt("material.diffuse", 0);
		shaderOne.setInt("material.specular", 1);
		shaderOne.setInt("material.emission", 2);

		//enable depth testing
		glEnable(GL_DEPTH_TEST);
	}
	void InitDraw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, Camera* drawCam, std::vector<Light*> lights) override {
		shaderOne.use();

		float time = glfwGetTime();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		/*glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissionMap);*/

		//define the ambient somehow
		shaderOne.setVec3("ambientLight", glm::vec3(0.1f, 0.1f, 0.1f));

		int pointLights = 0;
		//process the lights
		for (Light* l : lights)
		{
			switch (l->type)
			{
			case Directional:

				shaderOne.setVec3("dirLight.diffuse", l->diffuse);
				shaderOne.setVec3("dirLight.specular", l->specular);
				shaderOne.setVec3("dirLight.direction", l->direction);
				break;
			case Point:
				shaderOne.setFloat("pointLights[" + std::to_string(pointLights) + "].constant", l->attenuation.constant);
				shaderOne.setFloat("pointLights[" + std::to_string(pointLights) + "].linear", l->attenuation.linear);
				shaderOne.setFloat("pointLights[" + std::to_string(pointLights) + "].quadratic", l->attenuation.quadratic);
				shaderOne.setVec3("pointLights[" + std::to_string(pointLights) + "].position", l->position);
				shaderOne.setVec3("pointLights[" + std::to_string(pointLights) + "].diffuse", l->diffuse);
				shaderOne.setVec3("pointLights[" + std::to_string(pointLights) + "].specular", l->specular);

				pointLights++;
				break;
			case Spotlight:
				//TODO
				break;
			case Ambient:
				break;
			default:
				break;
			}
		}
		shaderOne.setInt("pointLightCount", pointLights);
		
		shaderOne.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		shaderOne.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		shaderOne.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		shaderOne.setFloat("material.shininess", 32.0f);

		/*
		float spotlightAngle = 22.5;
		float spotlightSmoothingRange = 6.0;
		shaderOne.setFloat("light.cutOff", glm::cos(glm::radians(spotlightAngle - spotlightSmoothingRange)));
		shaderOne.setFloat("light.outerCutOff", glm::cos(glm::radians(spotlightAngle)));*/

		//draw matrix variables
		shaderOne.setMat4("view", view);

		shaderOne.setMat4("projection", projection);

		shaderOne.setMat4("model", model);

	}
	Material_A() {};
};

