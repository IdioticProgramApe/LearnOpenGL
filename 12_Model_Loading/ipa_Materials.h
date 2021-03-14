#ifndef MATERIALS_H
#define MATERIALS_H

#include <glm.hpp>

struct Material
{
	int diffuse     { 0 };
	int specular    { 1 };
	float shininess { 32.0f };
};

namespace Materials
{
	constexpr Material defaultMaterial;
}

#endif // !MATERIALS_H

