#pragma once
#include <glm/matrix.hpp>
#include "Camera.h"
#include <vector>

class Drawable
{
public:
	virtual void Draw(class Camera* mainCam, class Material* mat, glm::mat4 objectTransform, std::vector<class Light*> lights) = 0;
};

//drawable needs to be included before any declarations of a class deriving from this
//still we want to include this here so that we dont have to include this in each deriving class separately
#include "Materials/Material.h"
