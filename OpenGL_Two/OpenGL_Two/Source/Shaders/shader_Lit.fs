#version 330 core
struct Material{
sampler2D diffuse;
sampler2D specular;
sampler2D emission;
float shininess;
};
struct DirLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 direction;
};
struct PointLight{
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

#define MAX_POINT_LIGHTS 64
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_emission1;

uniform vec3 ambientLight;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in mat4 View;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 result = vec3(0.0);
	
	//always ambient
	vec3 ambient = ambientLight * vec3(texture(texture_diffuse1, TexCoords));
	
	//emission 
	vec3 emission = vec3(1.0)*vec3(texture(texture_emission1, TexCoords));
	
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(-FragPos);
	//directional lighting
	result += CalcDirLight(dirLight, norm, viewDir);
	
	//point lights
	for(int i = 0; i< pointLightCount; i++)
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	
	result += ambient;
	//result += emission;
	
	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//No need to do these transformations every fragment?
	vec3 dir = vec3(View * vec4(light.direction, 0.0));
	
	vec3 lightDir = normalize(-dir);
	
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
		
	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),material.shininess);
	vec3 specular = vec3(texture(texture_specular1, TexCoords)) * spec * light.specular;
	
	return (diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//No need to do these transformations every fragment?
	vec3 lightPos = vec3(View * vec4(light.position, 1.0));
	vec3 lightDir = normalize(lightPos - fragPos);
	
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
		
	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),material.shininess);
	vec3 specular = vec3(texture(texture_specular1, TexCoords)) * spec * light.specular;
	
	//attenuation
	float dist = length(lightPos - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));    
	
	diffuse *= attenuation;
	specular *= attenuation;
	
	return(diffuse + specular);
}