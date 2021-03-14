#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char * vertexShaderPath, const char * fragmentShaderPath)
{
	/// load shaders, get the contents
	std::string vertexShader, fragmentShader;
	try
	{
		std::ifstream vertexShaderHandle, fragmentShaderHandle;
		auto readException = std::ifstream::badbit | std::ifstream::failbit;
		vertexShaderHandle.exceptions(readException);
		fragmentShaderHandle.exceptions(readException);

		// vertex shader
		vertexShaderHandle.open(vertexShaderPath);
		std::stringstream vertexShaderStream;
		vertexShaderStream << vertexShaderHandle.rdbuf();
		vertexShaderHandle.close();

		// fragment shader
		fragmentShaderHandle.open(fragmentShaderPath);
		std::stringstream fragmentShaderStream;
		fragmentShaderStream << fragmentShaderHandle.rdbuf();
		fragmentShaderHandle.close();

		vertexShader = vertexShaderStream.str();
		fragmentShader = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure & e)
	{
		std::cout << e.what() << std::endl;
	}

	/// create vertex fragment shader and compile them
	const auto compileShader = [](const char * source, GLenum type) -> GLuint
	{
		GLuint shaderID = glCreateShader(type);
		glShaderSource(shaderID, 1, &source, NULL);
		glCompileShader(shaderID);
		{
			GLint success;
			GLchar infoLog[512];
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
				std::string shaderType = "";
				switch (type)
				{
				case GL_VERTEX_SHADER:
					shaderType = "Vertex shader";
					break;
				case GL_FRAGMENT_SHADER:
					shaderType = "Fragment shader";
					break;
				default:
					break;
				}
				std::cout << shaderType << " compilation error: " << infoLog << std::endl;
			}
		}
	};

	GLuint vertexShaderID = compileShader(vertexShader.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID = compileShader(fragmentShader.c_str(), GL_FRAGMENT_SHADER);

	/// create a shader program to serialize the shaders
	m_id = glCreateProgram();
	glAttachShader(m_id, vertexShaderID);
	glAttachShader(m_id, fragmentShaderID);
	glLinkProgram(m_id);
	{
		GLint success;
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(m_id, 512, NULL, infoLog);
			std::cout << "Shader program linking error: " << infoLog << std::endl;
		}
	}

	/// after the program is successfully built, delete the shaders
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void Shader::use()
{
	glUseProgram(m_id);
}

void Shader::setInt(const GLchar * varName, int value)
{
	GLint varLoc = glGetUniformLocation(m_id, varName);
	glUniform1i(varLoc, value);
}

void Shader::setFloat(const GLchar * varName, float value)
{
	GLint varLoc = glGetUniformLocation(m_id, varName);
	glUniform1f(varLoc, value);
}

void Shader::setMat4(const GLchar * varName, const glm::mat4 & matrix)
{
	GLint varLoc = glGetUniformLocation(m_id, varName);
	glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}
