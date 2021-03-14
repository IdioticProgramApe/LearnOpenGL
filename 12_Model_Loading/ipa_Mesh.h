#ifndef MESH_H
#define MESH_H


#include <glm.hpp>

#include <string>
#include <sstream>
#include <vector>
#include "assimp/mesh.h"

#include "ipa_Shader.h"

using VertexArrayObj   = unsigned int;
using VertexBufferObj  = unsigned int;
using ElementBufferObj = unsigned int;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	unsigned int id;
	std::string  type;  // i.e. a diffuse map or a specular map
	aiString     path;
};

class Mesh
{
public:
	VertexArrayObj            m_VAO;
	std::vector<Vertex>       m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture>      m_textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh() {}
	void draw(Shader shader);

private:
	VertexBufferObj  m_VBO;
	ElementBufferObj m_EBO;
	void setupMesh();  // to initialize the buffers
};


#endif // ! MESH_H
