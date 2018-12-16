#pragma once
#include "Material.h"
#include "../stb_image.h"

class Material_A :
	public Material
{
public:
	void Initialize() override {
		shaderOne = Shader(
			"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Shaders/shader_Lit.vs",
			"G:/Projects/OpenGL/MinecraftClone/OpenGL_Graphical Engine/OpenGL_Two/OpenGL_Two/Source/Shaders/shader_Lit.fs");
		shaderOne.use();

		//enable depth testing
		glEnable(GL_DEPTH_TEST);
	}


	void InitDraw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, Camera* drawCam, std::vector<Light*> lights) override {
		shaderOne.use();

		float time = glfwGetTime();

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); //activate the proper tex unit before binding

			std::string number;
			std::string name = textures[i].type;

			if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
			else if (name == "texture_specular") number = std::to_string(specularNr++);
			else if (name == "texture_normal") number = std::to_string(normalNr++);
			else if (name == "texture_height") number = std::to_string(heightNr++);

			//This NEEDS to be setInt NOT setFloat!!!
			shaderOne.setInt((name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		//shininess
		shaderOne.setFloat("material.shininess", 32.0f);

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
		

		//draw matrix variables
		shaderOne.setMat4("view", view);

		shaderOne.setMat4("projection", projection);

		shaderOne.setMat4("model", model);

	}
	Material_A(){}
	Material_A(const Material_A &mat): Material(mat){}
};

