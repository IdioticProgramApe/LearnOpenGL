#ifndef MESH_H
#define MESH_H


#include <glm/glm.hpp>

#include <string>
#include <sstream>
#include <vector>

#include "shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;  // i.e. a diffuse map or a specular map
	aiString path;
};

class Mesh
{
public:
	unsigned int mVAO;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh() {};
	void Draw(Shader shader);

private:
	unsigned int mVBO, mEBO;
	void m_setupMesh();  // to initialize the buffers
};


#endif // ! MESH_H
