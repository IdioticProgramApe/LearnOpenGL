#include "callback.h"
#include "window.h"

bool firstMove{ true };
float lastX{ (float)Window::WIDTH / 2 }, lastY{ (float)Window::WIDTH / 2 };

void Callback::framebufferSize(GLFWwindow* window, GLint width, GLint height)
{
	glViewport(0, 0, width, height);
}

void Callback::cursorPos(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMove)
	{
		lastX = xPos;
		lastY = yPos;
		firstMove = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	// TODO: process xOffset and yOffset
}

void Callback::scroll(GLFWwindow* window, double xOffset, double yOffset)
{
	// TODO: process yOffset
}