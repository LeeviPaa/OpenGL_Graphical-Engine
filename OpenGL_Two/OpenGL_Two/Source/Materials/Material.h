#pragma once
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Shaders/Shader.h"
#include "../Debug.h"
#include "../Lighting/Light.h"

//material decides how a shader program is used, what textures are needed and such
//currently vertex shader NEEDS view, projection and model matrices to be drawn

class Material
{
protected:
	Shader shaderOne;

public:
	virtual void Initialize() = 0;
	virtual void InitDraw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, class Camera* drawCam, std::vector<Light*> lights) = 0;
};

