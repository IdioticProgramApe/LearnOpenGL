#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace utils
{
	void frameBufferSizeCallback(GLFWwindow*, int, int);
	void processInputs(GLFWwindow*);
	void checkShaderCompilation(GLuint);
	void checkProgramLink(GLuint);
}

#endif // !UTILS_H

