#version 330 core

struct DirectLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

uniform DirectLight light;

void main()
{
	FragColor = vec4(light.ambient + light.diffuse + light.specular, 1.0f);
}