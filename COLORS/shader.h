#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ShaderPaths
{
	constexpr auto VERTEX_L = "./shaders/light.vs";
	constexpr auto FRAGMENT_L = "./shaders/light.fs";

	constexpr auto VERTEX_O = "./shaders/object.vs";
	constexpr auto FRAGMENT_O = "./shaders/object.fs";
}

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	~Shader();

	void use();
	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void setVec3(const char* name, glm::vec3 vector);
	void setMat4(const char* name, glm::mat4 matrix);
};

#endif // !SHADER_H

