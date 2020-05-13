#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "image.h"
#include "camera.hpp"
#include "window.h"
#include "callback.h"
#include "shader.h"


void processInput(GLFWwindow* window);


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(Window::WIDTH, Window::HEIGHT, Window::TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "ERROR::WINDOW::CREATION" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// set callbacks
	glfwSetFramebufferSizeCallback(window, Callback::framebufferSize);
	glfwSetCursorPosCallback(window, Callback::cursorPos);
	glfwSetScrollCallback(window, Callback::scroll);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::PROC_ADDRESS::LOADING" << std::endl;
		return -1;
	}

	// create a shader
	Shader shaderProgram(ShaderPaths::VERTEX, ShaderPaths::FRAGMENT);


	glEnable(GL_DEPTH_TEST);
	shaderProgram.use();
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(Window::COLOR.r, Window::COLOR.g, Window::COLOR.b, Window::COLOR.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDisable(GL_DEPTH_TEST);

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}