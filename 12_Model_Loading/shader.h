#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "materials.h"
#include "lights.h"


class ReadFileHandler;  // RAII automatically close the buffer

class Shader
{
private:
	const char * m_vertShaderFile = "Vertex.glsl";
	const char * m_fragShaderFile = "Fragment.glsl";

	unsigned int m_id;
public:
	Shader();
	virtual ~Shader() = default;

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

