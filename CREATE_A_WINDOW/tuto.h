#ifndef TUTO_H
#define TUTO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "utils.h"

namespace tuto
{
	void framebuffer_size_callback(GLFWwindow*, int, int);
	void processInput(GLFWwindow*);
	int test();
}
#endif // !TUTO_H

