#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>


struct PointLight
{
	glm::vec3 position{ glm::vec3(1.2f, 1.0f, 2.0f) };

	glm::vec3 ambient{ glm::vec3(0.2f, 0.2f, 0.2f) };
	glm::vec3 diffuse{ glm::vec3(0.5f, 0.5f, 0.5f) };
	glm::vec3 specular{ glm::vec3(1.0f, 1.0f, 1.0f) };

	float constant{ 1.0f };
	float linear{ 0.09f };
	float quadratic{ 0.032f };
};

struct DirectLight
{
	glm::vec3 direction{ glm::vec3(-0.2f, -1.0f, -0.3f) };
	glm::vec3 ambient{ glm::vec3(0.2f, 0.2f, 0.2f) };
	glm::vec3 diffuse{ glm::vec3(0.5f, 0.5f, 0.5f) };
	glm::vec3 specular{ glm::vec3(1.0f, 1.0f , 1.0f) };
};

struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;
	float cutOff{ 12.5f };
	float outerCutOff{ 17.5 };

	glm::vec3 ambient{ glm::vec3(0.2f, 0.2f, 0.2f) };
	glm::vec3 diffuse{ glm::vec3(0.5f, 0.5f, 0.5f) };
	glm::vec3 specular{ glm::vec3(1.0f, 1.0f, 1.0f) };
};

namespace Lights
{
	constexpr PointLight defaultPointLight;
	constexpr DirectLight defaultDirectLight;
}

#endif // !LIGHTS_H
