#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform samplerCube skybox;
uniform vec3 camPos;

void main()
{             
    vec3 In = normalize(Position-camPos);
    vec3 Refl = reflect(In, normalize(Normal));
    FragColor = vec4(texture(skybox, Refl).rgb*0.9, 1.0);
}