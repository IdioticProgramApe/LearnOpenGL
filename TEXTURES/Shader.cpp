#include "Shader.h"


Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	std::string vertexSourceCode, fragmentSourceCode;
	std::ifstream vertexShaderFile, fragmentShaderFile;
	
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);

		std::stringstream vertexSourceStream, fragmentSourceStream;
		vertexSourceStream << vertexShaderFile.rdbuf();
		fragmentSourceStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexSourceCode = vertexSourceStream.str();
		fragmentSourceCode = fragmentSourceStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	int success;
	char infoLog[512];

	unsigned int vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertexSource = vertexSourceCode.c_str();
	const char* fragmentSource = fragmentSourceCode.c_str();

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram, vertexShader);
	glAttachShader(this->shaderProgram, fragmentShader);
	glLinkProgram(this->shaderProgram);
	glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
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
	glUseProgram(this->shaderProgram);
}

void Shader::setInt(const std::string &name, int value)
{
	GLint location = glGetUniformLocation(this->shaderProgram, name.c_str());
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, float value)
{
	GLint location = glGetUniformLocation(this->shaderProgram, name.c_str());
	glUniform1f(location, value);
}

void Shader::changeFloat(const std::string &name, float difference)
{
	GLint location = glGetUniformLocation(this->shaderProgram, name.c_str());
	GLfloat preValue;
	glGetUniformfv(this->shaderProgram, location, &preValue);
	glUniform1f(location, preValue + difference);
}
