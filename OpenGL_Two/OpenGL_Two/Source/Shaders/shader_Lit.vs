#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out mat4 View;

void main()
{
	TexCoords = aTexCoord;
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   FragPos = vec3(view * model*vec4(aPos, 1.0)); 
   
   //more expensive way to compensate for non uniform scale
   Normal = mat3(transpose(inverse(view * model)))*aNormal;
   View = view;
   //LightPos = vec3(view * vec4(lightPos, 1.0));
   //LightDir = vec3(view * vec4(lightDir, 0.0));
}