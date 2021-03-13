#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Singleton.h"

namespace WindowParams
{
	constexpr auto WIDTH  = 1920;
	constexpr auto HEIGHT = 1080;
	constexpr auto TITLE  = "12_MESH";

	constexpr auto COLOR  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

class Scene : public Singleton<Scene>
{
private:
	Camera m_camera;

	bool   m_firstMove = true;

	float  m_deltaTime = 0.0f;
	float  m_lastFrame = 0.0f;
	
	float  m_lastX     = WindowParams::WIDTH / 2.0f;
	float  m_lastY     = WindowParams::HEIGHT / 2.0f;

public:

	Camera & getCamera();

	bool & getFirstMove();
	float & getDeltaTime();
	float & getLastFrame();
	float & getLastX();
	float & getLastY();

	void processInput(GLFWwindow* window);
};

namespace Callback
{
	///
	void framebufferSize(GLFWwindow * window, GLint width, GLint height);
	///
	void cursorPos(GLFWwindow * window, double xPos, double yPos);
	///
	void scroll(GLFWwindow * window, double xOffset, double yOffset);
}

#endif // SCENE_H
