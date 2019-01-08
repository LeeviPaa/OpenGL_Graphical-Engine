#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitTangent;

uniform mat4 model;
layout (std140, binding = 0) uniform Matrices
{
	mat4 view;
	mat4 projection;
};
uniform float uvScale = 1;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out mat4 View;

void main()
{
	TexCoords = aTexCoord * uvScale;
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   FragPos = vec3(view * model*vec4(aPos, 1.0)); 
   
   //more expensive way to compensate for non uniform scale
   Normal = mat3(transpose(inverse(view * model)))*aNormal;
   View = view;
}