#include "utils.h"

namespace utils 
{
	// try to use file reading to replace the direct assignment
	std::string readFileIntoString(const char* filename)
	{
		std::ifstream in(filename);
		std::string str((std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>());
		return str;
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

	void checkShaderCompilation(GLuint shader)
	{
		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n"
				<< infoLog << std::endl;
		}
	}

	void checkProgramLink(GLuint program)
	{
		int success;
		char infoLog[512];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINK_FAILED\n"
				<< infoLog << std::endl;
		}
	}

	int test()
	{
		std::string str;
		str = utils::readFileIntoString("vertexShader.txt");
		std::cout << str << std::endl;
		printf(str.c_str(), "\n");
		return 0;
	}
}
