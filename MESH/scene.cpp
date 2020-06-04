#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>

#include "window.h"
#include "callback.h"
#include "shader.h"
#include "objects.h"
#include "camera.hpp"
#include "materials.h"
#include "lights.h"
#include "mesh.h"
#include "model.h"

#define LIGHT_COLOR_CAHNGE false

float deltaTime(0.0f), lastFrame(0.0f);
bool firstMove{ true };
float lastX{ (float)Window::WIDTH / 2 }, lastY{ (float)Window::WIDTH / 2 };
Camera cam(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

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

	// GLFW should capture the mouse movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::PROC_ADDRESS::LOADING" << std::endl;
		return -1;
	}

	// create shaders...
	Shader objectShader(ShaderPaths::OBJECT_V, ShaderPaths::OBJECT_F);

	Model ironMan(Objects::IRON_MAN);

	/*Texture diffuseMap(ImagePaths::CONTAINER);
	Texture specularMap(ImagePaths::CONTAINER_SPECULAR);*/

	// render...
	glm::mat4 model(1.0f), view(1.0f), projection(1.0f);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(Window::COLOR.r, Window::COLOR.g, Window::COLOR.b, Window::COLOR.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// update deltaTime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		projection = glm::perspective(glm::radians(cam.Zoom), (float)Window::WIDTH / Window::HEIGHT, 0.1f, 100.0f);

		objectShader.use();
		objectShader.setMaterial("material", Materials::defaultMaterial);
		objectShader.setDirectLight("directLight", Lights::defaultDirectLight);

		SpotLight spotLight{ cam.Position, cam.Front };
		objectShader.setSpotLight("spotLight", spotLight);

		objectShader.setVec3("viewPos", cam.Position);
		objectShader.setMat4("model", model);  // this model matrix is uniform matrix (no need for normal matrix)
		objectShader.setMat4("view", cam.getViewMatrix());
		objectShader.setMat4("projection", projection);

		ironMan.Draw(objectShader);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDisable(GL_DEPTH_TEST);

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam.processKeyboard(RIGHT, deltaTime);
}


// callbacks
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

	cam.processMouseMovement(xOffset, yOffset);

	lastX = xPos;
	lastY = yPos;
}

void Callback::scroll(GLFWwindow* window, double xOffset, double yOffset)
{
	cam.processMouseScroll(yOffset);
}