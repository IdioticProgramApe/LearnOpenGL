#include <iostream>
#include <stdio.h>

#include "shader.h"
#include "objects.h"
#include "materials.h"
#include "lights.h"
#include "mesh.h"
#include "model.h"
#include "Scene.h"


void Scene::processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_camera.processKeyboard(CameraMovement::FORWARD, m_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_camera.processKeyboard(CameraMovement::BACKWARD, m_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_camera.processKeyboard(CameraMovement::LEFT, m_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_camera.processKeyboard(CameraMovement::RIGHT, m_deltaTime);
	}
}

void framebufferSize(GLFWwindow * window, GLint width, GLint height)
{
	glViewport(0, 0, width, height);
}

void cursorPos(GLFWwindow * window, double xPos, double yPos)
{
	auto thisScene = Scene::getInstance();
	if (thisScene->m_firstMove)
	{
		thisScene->m_lastX = xPos;
		thisScene->m_lastY = yPos;
		thisScene->m_firstMove = false;
	}

	float xOffset = xPos - thisScene->m_lastX;
	float yOffset = thisScene->m_lastY - yPos;

	thisScene->m_camera.processMouseMovement(xOffset, yOffset);

	thisScene->m_lastX = xPos;
	thisScene->m_lastY = yPos;
}

void scroll(GLFWwindow * window, double xOffset, double yOffset)
{
	Scene::getInstance()->m_camera.processMouseScroll(yOffset);
}

int Scene::launch()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WindowParams::WIDTH, WindowParams::HEIGHT, WindowParams::TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "ERROR::WINDOW::CREATION" << std::endl;
		glfwTerminate();
		return -1;

	}
	glfwMakeContextCurrent(window);

	// set callbacks
	glfwSetFramebufferSizeCallback(window, framebufferSize);
	glfwSetCursorPosCallback(window, cursorPos);
	glfwSetScrollCallback(window, scroll);

	// GLFW should capture the mouse movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::PROC_ADDRESS::LOADING" << std::endl;
		return -1;
	}

	// create shaders...
	Shader objectShader(ShaderPaths::OBJECT_V, ShaderPaths::OBJECT_F);

	Model asset(Objects::NANO_SUIT);

	/*Texture diffuseMap(ImagePaths::CONTAINER);
	Texture specularMap(ImagePaths::CONTAINER_SPECULAR);*/

	// render...
	glm::mat4 model(1.0f), view(1.0f), projection(1.0f);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(WindowParams::COLOR.r, WindowParams::COLOR.g, WindowParams::COLOR.b, WindowParams::COLOR.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// update deltaTime
		float currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;

		processInput(window);

		projection = glm::perspective(glm::radians(m_camera.getZoom()),
			(float)WindowParams::WIDTH / WindowParams::HEIGHT, 0.1f, 100.0f);
		glm::mat4 objectModel = model;
		objectModel = glm::translate(objectModel, glm::vec3(0.0f));
		objectModel = glm::scale(objectModel, glm::vec3(1.0f));

		objectShader.use();
		objectShader.setDirectLight("directLight", Lights::defaultDirectLight);

		SpotLight spotLight{ m_camera.getPosition(), m_camera.getFront() };
		objectShader.setSpotLight("spotLight", spotLight);

		objectShader.setVec3("viewPos", m_camera.getPosition());
		objectShader.setMat4("model", objectModel);  // this model matrix is uniform matrix (no need for normal matrix)
		objectShader.setMat4("view", m_camera.getViewMatrix());
		objectShader.setMat4("projection", projection);

		asset.Draw(objectShader);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDisable(GL_DEPTH_TEST);

	glfwTerminate();

	return 0;
}
