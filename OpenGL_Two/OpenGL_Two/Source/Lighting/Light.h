#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Drawable.h"

enum LightType {
	Directional,
	Point,
	Spotlight,
	Ambient
};

struct Attenuation {
public:
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};

class Light: public Drawable
{
private:
	float cubeVertices[108] = {
		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f, 

		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 

		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  

		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f, 
	};

public:
	//who am I?
	LightType type = LightType::Directional;

	//all lights
	glm::vec3 diffuse = glm::vec3(0.0);
	glm::vec3 specular = glm::vec3(0.0);

	//lights with attenuation
	Attenuation attenuation;

	//spotlights
	float lightAngle = 25;
	float lightSmoothingAngle = 5;

	//we could inherit a transform class that contains these values
	glm::vec3 direction = glm::vec3(0.0, -1.0, 0.0);
	glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);

	void Draw(class Camera* mainCam, Material* mat, glm::mat4 objectTransform, std::vector<Light*> lights) override;
	unsigned int lightVAO;
	unsigned int lightVBO;
	Light(LightType lightType, glm::vec3 diff, glm::vec3 spec, glm::vec3 pos, glm::vec3 dir);
	void MakeSpotlight(float angle, float smoothingAngle);
	void InitModel();
};
