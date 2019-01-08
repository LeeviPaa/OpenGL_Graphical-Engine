#pragma once
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Shaders/Shader.h"
#include "../Debug.h"
#include "../Lighting/Light.h"
#include "../stb_image.h"
#include <string>
#include <vector>

//material decides how a shader program is used, what textures are needed and such
//currently vertex shader NEEDS view, projection and model matrices to be drawn

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Material
{
protected:
	//variables
	std::vector<Texture> textures;
	Shader shaderOne;

public:

	virtual void Initialize() = 0;
	//the view and projection matrices are set in a uniform buffer object
	virtual void InitDraw(glm::mat4 model, class Camera* drawCam, std::vector<Light*> lights) = 0;

	//constructors
	Material() {};
	Material(const Material &obj) {
		textures = obj.textures;
		shaderOne = obj.shaderOne;
	}

	//set shaders by ref or by path
	void SetShader(Shader shader)
	{
		shaderOne = shader;
	}
	void SetShader(std::string vertPath, std::string fragPath)
	{
		shaderOne = Shader(vertPath.c_str(), fragPath.c_str());
	}
	Shader* GetShader()
	{
		return &shaderOne;
	}

	//set textures by ref or by path
	void SetTextures(std::vector<Texture> text)
	{
		textures = text;
	}
	void setMaterialTexture(std::string path, aiTextureType type) {
		Texture texture;

		texture.id = TextureFromFile(path);
		texture.path = path;

		switch (type)
		{
		case aiTextureType::aiTextureType_DIFFUSE:
			texture.type = "texture_diffuse";
			break;
		case aiTextureType::aiTextureType_SPECULAR:
			texture.type = "texture_specular";
			break;
		case aiTextureType::aiTextureType_NORMALS:
			texture.type = "texture_normal";
			break;
		case aiTextureType::aiTextureType_HEIGHT:
			texture.type = "texture_height";
			break;
		default:
			break;
		}

		textures.push_back(texture);
	}

	unsigned int TextureFromFile(std::string directory)
	{
		std::string filename = directory;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;

		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			if (format == GL_RGBA)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			Debug::Log("Failed to load texture from: " + directory);
			stbi_image_free(data);
		}

		return textureID;
	}
};

