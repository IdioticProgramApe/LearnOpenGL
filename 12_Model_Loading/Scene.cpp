#include <iostream>
#include <stdio.h>

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
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_camera.processKeyboard(CameraMovement::UP, m_deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		m_camera.processKeyboard(CameraMovement::DOWN, m_deltaTime);
	}
}

Camera & Scene::getCamera()
{
	return m_camera;
}

bool & Scene::getFirstMove()
{
	return m_firstMove;
}

float & Scene::getDeltaTime()
{
	return m_deltaTime;
}

float & Scene::getLastFrame()
{
	return m_lastFrame;
}

float & Scene::getLastX()
{
	return m_lastX;
}

float & Scene::getLastY()
{
	return m_lastY;
}

void Callback::framebufferSize(GLFWwindow * window, GLint width, GLint height)
{
	glViewport(0, 0, width, height);
}

void Callback::cursorPos(GLFWwindow * window, double xPos, double yPos)
{
	if (Scene::getInstance().getFirstMove())
	{
		Scene::getInstance().getLastX() = xPos;
		Scene::getInstance().getLastY() = yPos;
		Scene::getInstance().getFirstMove() = false;
	}

	float xOffset = xPos - Scene::getInstance().getLastX();
	float yOffset = Scene::getInstance().getLastY() - yPos;

	Scene::getInstance().getCamera().processMouseMovement(xOffset, yOffset);

	Scene::getInstance().getLastX() = xPos;
	Scene::getInstance().getLastY() = yPos;
}

void Callback::scroll(GLFWwindow * window, double xOffset, double yOffset)
{
	Scene::getInstance().getCamera().processMouseScroll(yOffset);
}
