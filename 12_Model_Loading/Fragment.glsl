#version 330 core

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
};

struct DirectLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;	
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 viewPos;
uniform DirectLight directLight;
uniform SpotLight spotLight;
uniform Material material;

vec3 calculateDirectLight(DirectLight light, vec3 normal, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 color;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 normal = normalize(Normal);

	color += calculateDirectLight(directLight, normal, viewDir);
	color += calculateSpotLight(spotLight, normal, FragPos, viewDir);

	FragColor = vec4(color, 1.0f);
}

vec3 calculateDirectLight(DirectLight light, vec3 normal, vec3 viewDir)
{
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));

	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), 32.0f);
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

	return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.texture_diffuse1, TexCoords)));

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
	vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, TexCoords)));

	return (ambient + (diffuse + specular) * intensity);
}