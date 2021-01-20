#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader
{
public:
	unsigned int shaderProgram;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	void use();
	void setInt(const GLchar* name, GLint value);
	void setFloat(const GLchar* name, GLfloat value);
	void setMat4(const GLchar* name, glm::mat4 trans);
	~Shader();
};


#endif // !SHADER_H


