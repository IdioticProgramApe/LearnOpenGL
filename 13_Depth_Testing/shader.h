#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
private:
	GLuint m_id = 0;

public:
	Shader() = delete;
	Shader(const char * vertexShaderPath, const char * fragmentShaderPath);

	virtual ~Shader();

	void use();

	void setInt(const GLchar * varName, int value);
	void setFloat(const GLchar * varName, float value);
	void setMat4(const GLchar * varName, const glm::mat4 & matrix);
};

#endif

