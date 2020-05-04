#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
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
	~Shader();
};


#endif // !SHADER_H


