#include "Shader.h"


Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{

}


Shader::~Shader()
{
}

void Shader::use()
{
	glUseProgram(this->shaderProgram);
}
