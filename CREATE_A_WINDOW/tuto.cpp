#include "tuto.h"
#include "constants.h"

namespace tuto
{
	int test()
	{
		glfwInit();

		// check the GLFW version, 3.3 and core profile
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// initialize an OpenGL window
		GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		// check GLAD initialization
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		// set viewport
		glViewport(0, 0, 800, 600);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		// generate a buffer ID for VBO: vertex buffer objects
		unsigned int VBO;
		glGenBuffers(1, &VBO);

		// advanced, generate a EBO: element buffer objects
		unsigned int EBO;
		glGenBuffers(1, &EBO);

		// generate a VAO to store all status for a drawing: vertex array objects
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);

		// setup shader program: vertex, fragment, program
		// compile a veretx shader
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &constants::vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// check shader compiling
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				<< infoLog << std::endl;
		}

		//compile a fragment shader
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &constants::fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		// create a shader program and link the shaders to it
		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
				<< infoLog << std::endl;
		}
		// the deletion can be done after the link
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// .. :: Initialization code (done once (unless the object frequently change)) :: ..
		// 1. bind Vertex Array Object
		glBindVertexArray(VAO);
		// 2. copy the vertices array in a vertex buffer for OPENGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(constants::vertices), constants::vertices, GL_STATIC_DRAW);
		// 3. copy the index array in a element buffer for OPENGL to use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(constants::indices), constants::indices, GL_STATIC_DRAW);
		// 4. then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// rendering loop
		while (!glfwWindowShouldClose(window))
		{
			// clear the buffer
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// input
			processInput(window);

			// .. :: Drawing code :: ..
			// 5. draw the object
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			// glDrawArrays(GL_TRIANGLES, 0, 3);

			// wireframe mode, draw the wireframe with lines
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			// default: fill mode
			// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);  // unbind

			// check and call events and swap the buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		glfwTerminate();

		return 0;
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
	}
}
