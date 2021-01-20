#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "materials.h"
#include "lights.h"

namespace ShaderPaths
{
	constexpr auto POINT_LIGHT_V = "./shaders/pointLight.vs";
	constexpr auto POINT_LIGHT_F = "./shaders/pointLight.fs";

	constexpr auto OBJECT_V = "./shaders/object.vs";
	constexpr auto POINT_OBJECT_F = "./shaders/pointObject.fs";
	constexpr auto DIRECT_OBJECT_F = "./shaders/directObject.fs";
	constexpr auto SPOT_OBJECT_F = "./shaders/spotObject.fs";
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
	void setMaterial(const char* name, Material material);
	void setPointLight(const char* name, PointLight pointLight);
	void setDirectLight(const char* name, DirectLight directLight);
	void setSpotLight(const char* name, SpotLight spotLight);
};

#endif // !SHADER_H

