#pragma once
#include "../Drawable.h"
#include <glm/glm.hpp>
#include <string>
#include "../Materials/Material.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

class Mesh : public Drawable
{
private:
	unsigned int VBO, EBO;
	void SetupMesh();

public:
	unsigned int VAO;

	std::vector<Vertex>	vertices;
	std::vector<unsigned int> indices;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void Draw(class Camera* mainCam, Material* mat, glm::mat4 objectTransform, std::vector<Light*> lights) override;
};

