#include "Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexShaderSource, fragmentShaderSource;
	std::ifstream vertexShaderBuffer, fragmentShaderBuffer;
	
	vertexShaderBuffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderBuffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertexShaderBuffer.open(vertexShaderPath);
		fragmentShaderBuffer.open(fragmentShaderPath);

		std::stringstream vertexShaderStream, fragmentShaderStream;
		vertexShaderStream << vertexShaderBuffer.rdbuf();
		fragmentShaderStream << fragmentShaderBuffer.rdbuf();

		vertexShaderBuffer.close();
		fragmentShaderBuffer.close();

		vertexShaderSource = vertexShaderStream.str();
		fragmentShaderSource = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE::READ" << std::endl;
	}
	int success;
	char infoLog[512];

	unsigned int vertexShader, fragmentShader;
	// build vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderCString = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderCString, NULL);
	glCompileShader(vertexShader);
	// check compilation
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FROM_SOURCE"
			<< infoLog << std::endl;
	}

	// build fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderCString = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderCString, NULL);
	glCompileShader(fragmentShader);

	// check compilation
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FROM_SOURCE"
			<< infoLog << std::endl;
	}

	// link the shaders to the program
	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram, vertexShader);
	glAttachShader(this->shaderProgram, fragmentShader);
	glLinkProgram(this->shaderProgram);
	// check linking
	glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED"
			<< infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(this->shaderProgram);
}

void Shader::setInt(const GLchar* name, GLint value)
{
	GLint location = glGetUniformLocation(this->shaderProgram, name);
	glUniform1i(location, value);
}

void Shader::setFloat(const GLchar* name, GLfloat value)
{
	GLint location = glGetUniformLocation(this->shaderProgram, name);
	glUniform1f(location, value);
}

void Shader::setMat4(const GLchar* name, glm::mat4 trans)
{
	GLint location = glGetUniformLocation(this->shaderProgram, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(trans));
}

Shader::~Shader()
{
}