#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "tuto.h"
#include "utils.h"
#include "Shader.h"

#define EXERCISE 0
/*
0: tuto example
1: first exercise, page 57
2: second exercise, page 57
3: third exercise, page 57
*/


int tuto::colorAttrib()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "tuto2-color attributes", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, utils::frameBufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

#if EXERCISE == 1
	// First exercise
	float vertices[] = {
		// positions        // colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
	   -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top
	};
	Shader myShader("./shaderWithColorUpsideDown.vs", "./shaderWithColor.fs");
#elif EXERCISE == 2
	// Second exercise
	float vertices[] = {
		// positions        // colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
	   -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top
	};
	Shader myShader("./shaderWithColorHOffset.vs", "./shaderWithColor.fs");
#elif EXERCISE == 3
	float vertices[] = {
		// positions
		0.5f, -0.5f, 0.0f, // bottom right
	   -0.5f, -0.5f, 0.0f, // bottom left
		0.0f,  0.5f, 0.0f  // top
	};

	/* a possible answer:
	all negative values are treated like 0s, 
	since the bottom left point's coords are all non-pos,
	therefore it's rendered black.
	the color interpolation should happen before the clipping. */

	Shader myShader("./shaderWithColorEqPos.vs", "./shaderWithColorEqPos.fs");
#else
	float vertices[] = {
		// positions        // colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
	   -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top
	};
	Shader myShader("./shaderWithColor.vs", "./shaderWithColor.fs");
#endif // EXERCISE == 1

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

#if EXERCISE == 3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
#else
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
#endif // EXERCISE == 3

	while (!glfwWindowShouldClose(window))
	{
		utils::processInputs(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myShader.use();
#if EXERCISE == 2
		myShader.setFloat("hOffset", 0.4f);
#endif // EXERCISE == 2
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();

	return 0;
}