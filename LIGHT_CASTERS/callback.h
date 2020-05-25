#ifndef CALLBACK_H
#define CALLBACK_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Callback
{
	void framebufferSize(GLFWwindow* window, GLint width, GLint height);
	void cursorPos(GLFWwindow* window, double xPos, double yPos);
	void scroll(GLFWwindow* window, double xOffset, double yOffset);
}

#endif // !CALLBACK_H
