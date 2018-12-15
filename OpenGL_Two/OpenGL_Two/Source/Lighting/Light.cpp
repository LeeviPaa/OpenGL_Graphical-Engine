#include "Light.h"
#include "../Materials/material_Light.h"

Light::Light(LightType lightType, glm::vec3 diff, glm::vec3 spec, glm::vec3 pos, glm::vec3 dir)
{
	type = lightType;
	diffuse = diff;
	specular = spec;

	position = pos;
	direction = dir;

	InitModel();
}

void Light::MakeSpotlight(float angle, float smoothingAngle)
{
	type = LightType::Spotlight;
	lightAngle = angle;
	lightSmoothingAngle = smoothingAngle;
}

void Light::InitModel()
{
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);

	//copy vertices to array in VBO
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Light::Draw(class Camera* mainCam, Material* mat, glm::mat4 objectTransform, std::vector<class Light*> lights)
{
	//camera
	glm::mat4 view;
	view = mainCam->GetViewMatrix();
	glm::mat4 proj;
	proj = mainCam->GetProjection();

	//initialize material properties (textures and such) because each material has its own properties and textures to set
	//"material, get ready to be drawn!"
	mat->InitDraw(view, proj, objectTransform, mainCam, lights);

	glBindVertexArray(lightVAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

