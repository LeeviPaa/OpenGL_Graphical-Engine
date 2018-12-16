#pragma once
#include <vector>
#include "../Drawable.h"
#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <map>
#include "../stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Materials/Material_A.h"

class Model: public Drawable
{
private:
	//these two sync materials & meshes
	std::vector<Mesh> meshes;
	std::vector<Material_A> materials;

	std::string directory;
	std::vector<Texture> textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	Material_A processMaterial(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char *path, const std::string &directory);
public:
	Model(std::string const &path)
	{
		loadModel(path);
	}
	void Draw(class Camera* mainCam, Material* mat, glm::mat4 objectTransform, std::vector<Light*> lights) override;
	std::vector<Material_A> GetMaterials()
	{
		return materials;
	}
};

