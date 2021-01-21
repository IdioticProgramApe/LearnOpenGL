#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class ReadFileHandler
{
private:
	std::string m_content;
	std::ifstream m_buffer;
	std::stringstream m_stream;

public:
	ReadFileHandler(const char * path)
	{
		m_buffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		m_buffer.open(path);
		m_stream << m_buffer.rdbuf();

		m_content = m_stream.str();
	}

	virtual ~ReadFileHandler() { m_buffer.close(); }

	std::string getContent() const
	{
		return m_content;
	}
};


Shader::Shader()
{
	std::string vertexString, fragmentString;
	try
	{
		// read from vertex shader file
		{
			ReadFileHandler vertexShaderHandler(m_vertShaderFile);
			vertexString = vertexShaderHandler.getContent();
		}

		// read from fragment shader file
		{
			ReadFileHandler fragmentShaderHandler(m_fragShaderFile);
			fragmentString = fragmentShaderHandler.getContent();
		}
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

	m_id = glCreateProgram();
	glAttachShader(m_id, vertexShader);
	glAttachShader(m_id, fragmentShader);
	glLinkProgram(m_id);
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_id, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK: "
			<< infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(m_id);
}

void Shader::setInt(const char* name, int value)
{
	GLint location = glGetUniformLocation(m_id, name);
	glUniform1i(location, value);
}

void Shader::setFloat(const char* name, float value)
{
	GLint location = glGetUniformLocation(m_id, name);
	glUniform1f(location, value);
}

void Shader::setVec3(const char* name, glm::vec3 vector)
{
	GLint location = glGetUniformLocation(m_id, name);
	glUniform3fv(location, 1, glm::value_ptr(vector));
}

void Shader::setMat4(const char* name, glm::mat4 matrix)
{
	GLint location = glGetUniformLocation(m_id, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMaterial(const char* name, Material material)
{
	std::string nameString = name;
	setInt((nameString + (std::string)".diffuse").c_str(), material.diffuse);
	setInt((nameString + (std::string)".specular").c_str(), material.specular);
	setFloat((nameString + (std::string)".shininess").c_str(), material.shininess);
}

void Shader::setPointLight(const char* name, PointLight pointLight)
{
	std::string nameString = name;
	setVec3((nameString + (std::string)".position").c_str(), pointLight.position);

	setVec3((nameString + (std::string)".ambient").c_str(), pointLight.ambient);
	setVec3((nameString + (std::string)".diffuse").c_str(), pointLight.diffuse);
	setVec3((nameString + (std::string)".specular").c_str(), pointLight.specular);

	setFloat((nameString + (std::string)".constant").c_str(), pointLight.constant);
	setFloat((nameString + (std::string)".linear").c_str(), pointLight.linear);
	setFloat((nameString + (std::string)".quadratic").c_str(), pointLight.quadratic);
}

void Shader::setDirectLight(const char* name, DirectLight directLight)
{
	std::string nameString = name;
	setVec3((nameString + (std::string)".direction").c_str(), directLight.direction);
	setVec3((nameString + (std::string)".ambient").c_str(), directLight.ambient);
	setVec3((nameString + (std::string)".diffuse").c_str(), directLight.diffuse);
	setVec3((nameString + (std::string)".specular").c_str(), directLight.specular);
}

void Shader::setSpotLight(const char* name, SpotLight spotLight)
{
	std::string nameString = name;
	setVec3((nameString + (std::string)".position").c_str(), spotLight.position);
	setVec3((nameString + (std::string)".direction").c_str(), spotLight.direction);
	setFloat((nameString + (std::string)".cutOff").c_str(), glm::cos(glm::radians(spotLight.cutOff)));
	setFloat((nameString + (std::string)".outerCutOff").c_str(), glm::cos(glm::radians(spotLight.outerCutOff)));

	setVec3((nameString + (std::string)".ambient").c_str(), spotLight.ambient);
	setVec3((nameString + (std::string)".diffuse").c_str(), spotLight.diffuse);
	setVec3((nameString + (std::string)".specular").c_str(), spotLight.specular);
}
