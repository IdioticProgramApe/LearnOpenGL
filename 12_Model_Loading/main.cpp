#define STB_IMAGE_IMPLEMENTATION

#include "ipa_Scene.h"
#include "ipa_Shader.h"
#include "ipa_Objects.h"
#include "ipa_Materials.h"
#include "ipa_Lights.h"
#include "ipa_Mesh.h"
#include "ipa_Model.h"


int main()
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
	Shader objectShader;
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
		Scene::getInstance().getDeltaTime() = currentFrame - Scene::getInstance().getLastFrame();
		Scene::getInstance().getLastFrame() = currentFrame;

		Scene::getInstance().processInput(window);

		projection = glm::perspective(glm::radians(Scene::getInstance().getCamera().getZoom()),
			(float)WindowParams::WIDTH / WindowParams::HEIGHT, 0.1f, 100.0f);
		glm::mat4 objectModel = model;
		objectModel = glm::translate(objectModel, glm::vec3(0.0f, -5.0f, -15.0f));
		objectModel = glm::scale(objectModel, glm::vec3(0.5f));

		objectShader.use();
		objectShader.setDirectLight("directLight", Lights::defaultDirectLight);

		SpotLight spotLight{ Scene::getInstance().getCamera().getPosition(), Scene::getInstance().getCamera().getFront() };
		objectShader.setSpotLight("spotLight", spotLight);

		// in Fragment shader
		objectShader.setVec3("viewPos", Scene::getInstance().getCamera().getPosition());

		// in vertex shader
		objectShader.setMat4("model", objectModel);  // this model matrix is uniform matrix (no need for normal matrix)
		objectShader.setMat4("view", Scene::getInstance().getCamera().getViewMatrix());
		objectShader.setMat4("projection", projection);

		asset.draw(objectShader);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDisable(GL_DEPTH_TEST);

	glfwTerminate();

	return 0;
}