#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int shaderProgram;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use();
	~Shader();
};

#endif // !SHADER_H

