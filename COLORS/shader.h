#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

namespace ShaderPaths
{
	constexpr auto VERTEX = "./shaders/vertex.vs";
	constexpr auto FRAGMENT = "./shaders/fragment.fs";
}

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexShaderPath = ShaderPaths::VERTEX, const char* fragmentShaderPath = ShaderPaths::FRAGMENT);
	~Shader();

	void use();
	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
};

#endif // !SHADER_H

