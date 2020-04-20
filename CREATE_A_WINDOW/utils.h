#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "utils.h"

namespace utils {
	std::string readFileIntoString(const char*);
	void framebuffer_size_callback(GLFWwindow*, int, int);
	void processInput(GLFWwindow*);
	void checkShaderCompilation(GLuint);
	void checkProgramLink(GLuint);
	int test();
}

#endif // !UTILS_H
