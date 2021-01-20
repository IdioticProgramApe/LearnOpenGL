#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Singleton.h"

namespace WindowParams
{
	constexpr auto WIDTH = 800;
	constexpr auto HEIGHT = 600;
	constexpr auto TITLE = "12_MESH";

	constexpr auto COLOR = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}


class Scene : public Singleton<Scene>
{
public:
	Camera m_camera;

	bool m_firstMove = true;

	float m_deltaTime = 0.0f;
	float m_lastFrame = 0.0f;
	
	float m_lastX = WindowParams::WIDTH / 2.0f;
	float m_lastY = WindowParams::HEIGHT / 2.0f;

private:
	void processInput(GLFWwindow* window);

public:
	//callbacks
	friend void framebufferSize(GLFWwindow* window, GLint width, GLint height);
	friend void cursorPos(GLFWwindow* window, double xPos, double yPos);
	friend void scroll(GLFWwindow* window, double xOffset, double yOffset);

public:
	Scene() = default;
	virtual ~Scene() = default;

	int launch();
};

#endif // SCENE_H
