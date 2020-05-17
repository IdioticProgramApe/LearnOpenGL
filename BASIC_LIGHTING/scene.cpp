#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "image.h"
#include "window.h"
#include "callback.h"
#include "shader.h"
#include "objects.h"
#include "camera.hpp"

#define MOVING_LIGHT true

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::PROC_ADDRESS::LOADING" << std::endl;
		return -1;
	}

	// create a shader
	Shader objectShader(ShaderPaths::VERTEX_O, ShaderPaths::FRAGMENT_O);
	Shader lightShader(ShaderPaths::VERTEX_L, ShaderPaths::FRAGMENT_L);

	// configure the buffers: VBO and VAO
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Objects::CUBE), Objects::CUBE, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// TODO: bind texture to the object
	unsigned char* iData;
	int iWidth, iHeight, iNrChannels;
	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	iData = stbi_load(Images::CONSTAINER, &iWidth, &iHeight, &iNrChannels, NULL);
	if (iData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, iData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::IMAGE::LOAD_FROM_FILE" << std::endl;
	}
	stbi_image_free(iData);

	// set up a lighting... (using a new VAO)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // since VBO stores the correct data already
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// render...
	glm::mat4 model(1.0f), view(1.0f), projection(1.0f);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

#if MOVING_LIGHT
		glm::vec3 light = glm::vec3(Objects::LIGHT.x * sin((float)glfwGetTime()), Objects::LIGHT.y, Objects::LIGHT.z * cos((float)glfwGetTime()));
#else
		glm::vec3 light = Objects::LIGHT;
#endif

		projection = glm::perspective(glm::radians(cam.Zoom), (float)Window::WIDTH / Window::HEIGHT, 0.1f, 100.0f);

		// draw light
		glm::mat4 lightModel = glm::translate(model, light);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		lightShader.use();
		lightShader.setMat4("model", lightModel);
		lightShader.setMat4("view", cam.getViewMatrix());
		lightShader.setMat4("projection", projection);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(Objects::CUBE) / sizeof(float));
		glBindVertexArray(0);

		// draw object
		objectShader.use();
		glm::mat4 objectModel = model;
		objectShader.setVec3("objectColor", Objects::COLOR_O);
		objectShader.setVec3("lightColor", Objects::COLOR_L);
		objectShader.setVec3("lightPos", light);
		objectShader.setVec3("viewPos", cam.Position);
		objectShader.setMat4("model", objectModel);  // this model matrix is uniform matrix (no need for normal matrix)
		objectShader.setMat4("view", cam.getViewMatrix());
		objectShader.setMat4("projection", projection);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(Objects::CUBE) / sizeof(float));
		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDisable(GL_DEPTH_TEST);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);

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

	// TODO: process xOffset and yOffset
	cam.processMouseMovement(xOffset, yOffset);
}

void Callback::scroll(GLFWwindow* window, double xOffset, double yOffset)
{
	// TODO: process yOffset
	cam.processMouseScroll(yOffset);
}