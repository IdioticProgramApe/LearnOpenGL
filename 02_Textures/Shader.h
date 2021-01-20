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
	void setInt(const std::string &name, int value);
	void setFloat(const std::string &name, float value);
	void changeFloat(const std::string &name, float value);
	~Shader();
};

#endif // !SHADER_H

