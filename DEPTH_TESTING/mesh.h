#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	// ref: http://www.opengl-tutorial.org/cn/intermediate-tutorials/tutorial-13-normal-mapping/
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	// mesh data
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
	unsigned int VAO;

	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	{
		m_vertices = vertices;
		m_indices = indices;  // draw elements needs indices (mapping to vertices)
		m_textures = textures;

		// setup the vertex buffers and its attribute pointers
		setupMesh();
	}

	// render the mesh
	void draw(Shader &shader)
	{
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < m_textures.size(); ++i)
		{
			// activate the proper texture, before binding
			glActiveTexture(GL_TEXTURE0 + i);

			// get the texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = m_textures[i].type;
			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular")
			{
				number = std::to_string(specularNr++);
			}
			else if (name == "texture_normal")
			{
				number = std::to_string(normalNr++);
			}
			else if (name == "texture_height")
			{
				number = std::to_string(heightNr++);
			}

			// now set the sampler to the correct texture unit 
			// need to check the shader code
			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);

			// bind the texture
			glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		}

		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		
		// after drawing, unbind the VAO
		glBindVertexArray(0);

		// Good Practice: set everything back to defaults once configured
		glActiveTexture(GL_TEXTURE0);
	}

private:
	// vertex buffer and element buffer
	unsigned int VBO, EBO;

	// initialize all the buffer objects/array
	void setupMesh()
	{
		// create buffers/arrays
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
	}
};


#endif // !MESH_H
