#ifndef WINDOW_H
#define WINDOW_H

#include <glm/glm.hpp>

namespace Window
{
	constexpr auto WIDTH  = 800;
	constexpr auto HEIGHT = 600;
	constexpr auto TITLE  = "BASIC LIGHTING";

	constexpr auto COLOR = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
}

#endif // !WINDOW_H
