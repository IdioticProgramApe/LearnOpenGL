#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexShaderPath = ShaderPaths::VERTEX, const char* fragmentShaderPath = ShaderPaths::FRAGMENT)
{
	// read script
	std::ifstream vertexBuffer, fragmentBuffer;
	std::string vertexString, fragmentString;

	vertexBuffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentBuffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try
	{
		// to store the buffer stream
		std::stringstream vertexStream, fragmentStream;

		vertexBuffer.open(vertexShaderPath);
		vertexStream << vertexBuffer.rdbuf();
		vertexBuffer.close();
		vertexString = vertexStream.str();

		fragmentBuffer.open(fragmentShaderPath);
		fragmentStream << fragmentBuffer.rdbuf();
		fragmentBuffer.close();
		fragmentString = fragmentStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_READ" << std::endl;
	}

	int success;
	char infoLog[512];

	const char* vertexSource = vertexString.c_str();
	const char* fragmentSource = fragmentString.c_str();
	unsigned int vertexShader, fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION::VERTEX: "
			<< infoLog << std::endl;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMIPLATION::FRAGMENT: "
			<< infoLog << std::endl;
	}

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertexShader);
	glAttachShader(this->ID, fragmentShader);
	glLinkProgram(this->ID);
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK: "
			<< infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


Shader::~Shader()
{
}

void Shader::use()
{
	glUseProgram(this->ID);
}
