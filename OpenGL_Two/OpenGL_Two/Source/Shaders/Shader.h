#ifndef SHADER_H
#define SHADER_H
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum RenderType
{
	Opaque,
	Transparent,
	Cutout
};

class Shader
{
protected:
	RenderType rendType = RenderType::Opaque;
public:
	unsigned int ID;
	Shader() {}
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const Shader& shader)
	{
		ID = shader.ID;
	}
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string &name, glm::mat4 value) const;
	void setVec3(const std::string &name, glm::vec3 value) const;
	void setRenderType(RenderType type);
	RenderType GetRenderType()
	{
		return rendType;
	}

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};
#endif