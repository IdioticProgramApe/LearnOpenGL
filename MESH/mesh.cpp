#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;

	m_setupMesh();
}

void Mesh::Draw(Shader shader)
{
	unsigned int diffuseNb = 1;
	unsigned int specularNb = 1;
	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		// set uniform sampler2D (textures) in shader
		std::stringstream ss;
		std::string number;
		std::string name = m_textures[i].type;
		if (name == "tetxure_diffuse") ss << diffuseNb++;
		else if (name == "texture_specular") ss << specularNb++;
		number = ss.str();

		shader.setInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0); // deactivate textures

	// draw meshes
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::m_setupMesh()
{
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
	glGenVertexArrays(1, &mVAO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);  // end of vertex array binding
}
