#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform samplerCube skybox;
uniform vec3 camPos;

void main()
{             
    float ratio = 1.00 / 1.52;
    vec3 In = normalize(Position - camPos);
    vec3 Refl = refract(In, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, Refl).rgb, 1.0);
}  