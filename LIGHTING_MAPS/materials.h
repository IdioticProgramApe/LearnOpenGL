#ifndef MATERIALS_H
#define MATERIALS_H

#include <glm/glm.hpp>

struct Material
{
	int diffuse{ 0 };
	glm::vec3 specular{ glm::vec3(0.5f, 0.5f, 0.5f) };
	float shininess{ 32.0f };
};

namespace Materials
{
	constexpr Material defaultMaterial;
}

#endif // !MATERIALS_H

