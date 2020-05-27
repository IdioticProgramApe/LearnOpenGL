#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>

#include "image.hpp"
#include "window.h"
#include "callback.h"
#include "shader.h"
#include "objects.h"
#include "camera.hpp"
#include "materials.h"
#include "lights.h"

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::PROC_ADDRESS::LOADING" << std::endl;
		return -1;
	}

	// create shaders...
	Shader objectShader(ShaderPaths::OBJECT_V, ShaderPaths::OBJECT_F);
	Shader lightShader(ShaderPaths::POINT_LIGHT_V, ShaderPaths::POINT_LIGHT_F);

	// configure the buffers: VBO and VAO
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Objects::CUBE_PRIME), Objects::CUBE_PRIME, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// configure the buffer: lightVAO
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Texture diffuseMap(ImagePaths::CONTAINER);
	Texture specularMap(ImagePaths::CONTAINER_SPECULAR);

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

		projection = glm::perspective(glm::radians(cam.Zoom), (float)Window::WIDTH / Window::HEIGHT, 0.1f, 100.0f);

		for (auto lightPos : Lights::pointLightPositions)
		{
			// draw light object
			lightShader.use();
			glm::mat4 lightModel = model;
			lightModel = glm::translate(lightModel, lightPos);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			lightShader.setDirectLight("light", Lights::defaultDirectLight);
			lightShader.setMat4("projection", projection);
			lightShader.setMat4("view", cam.getViewMatrix());
			lightShader.setMat4("model", lightModel);

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		// draw object
		objectShader.use();
		int cubeIdx = 0;
		for (auto cubePosition : Objects::CUBE_POSITIONS)
		{
			glm::mat4 objectModel = model;
			objectModel = glm::translate(objectModel, cubePosition);
			objectModel = glm::rotate(objectModel, glm::radians(20.0f * cubeIdx), glm::vec3(1.0f, 0.3f, 0.5f));
			objectShader.setMaterial("material", Materials::defaultMaterial);

			objectShader.setDirectLight("directLight", Lights::defaultDirectLight);

			int lightCnt = 0;
			for (auto lightPos : Lights::pointLightPositions)
			{
				PointLight pointLight{ lightPos };
				char pointLightName[16];
				sprintf_s(pointLightName, 16, "pointLights[%d]", lightCnt++);
				objectShader.setPointLight(pointLightName, pointLight);
			}

			SpotLight spotLight{ cam.Position, cam.Front };
			objectShader.setSpotLight("spotLight", spotLight);

#if LIGHT_COLOR_CAHNGE
			glm::vec3 lightColor;
			lightColor.x = sin((float)glfwGetTime() * 2.0f);
			lightColor.y = sin((float)glfwGetTime() * 0.7f);
			lightColor.z = sin((float)glfwGetTime() * 1.0f);
			objectShader.setVec3("light.ambient", lightColor * glm::vec3(0.2f));
			objectShader.setVec3("light.diffuse", lightColor * glm::vec3(0.5f));
#endif
			objectShader.setVec3("viewPos", cam.Position);
			objectShader.setMat4("model", objectModel);  // this model matrix is uniform matrix (no need for normal matrix)
			objectShader.setMat4("view", cam.getViewMatrix());
			objectShader.setMat4("projection", projection);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap.getTextureID());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap.getTextureID());
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			cubeIdx += 1;
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDisable(GL_DEPTH_TEST);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

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

	lastX = xPos;
	lastY = yPos;
}

void Callback::scroll(GLFWwindow* window, double xOffset, double yOffset)
{
	cam.processMouseScroll(yOffset);
}