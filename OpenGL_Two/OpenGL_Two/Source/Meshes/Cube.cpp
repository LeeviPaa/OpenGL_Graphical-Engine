#include "Cube.h"

#include "../stb_image.h"

glm::vec3 cubePositionss[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

void Cube::Initialize()
{
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	//square setup, bind VAO
	glBindVertexArray(VAO);

	//copy vertices to array in VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	//copy indices to array in element buffer
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);*/

	//set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Cube::Draw(Camera* mainCam, Material* mat, glm::mat4 objectTransform, std::vector<class Light*> lights)
{
	//we dont cull these because the (my defined) vertex indices suck and dont work
	glDisable(GL_CULL_FACE);

	//camera
	glm::mat4 view;
	view = mainCam->GetViewMatrix();
	glm::mat4 proj;
	proj = mainCam->GetProjection();

	//initialize material properties (textures and such) because each material has its own properties and textures to set
	//"material, get ready to be drawn!"
	mat->InitDraw(view, proj, objectTransform, mainCam, lights);

	glBindVertexArray(VAO);


	glDrawArrays(GL_TRIANGLES, 0, 36);
}

Cube::~Cube()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
