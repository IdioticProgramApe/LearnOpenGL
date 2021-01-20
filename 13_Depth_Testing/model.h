#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


unsigned int textureFromFile(const char* path, const std::string &directory, bool gamma = false);

class Model
{
public:
	// model data
	std::vector<Texture> m_texturesLoaded;
	std::vector<Mesh> m_meshes;
	std::string m_directory;
	bool m_gammaCorrection;

	// constructor, expects a filepath to a 3D model
	Model(const char* path, bool gamma = false) : m_gammaCorrection(gamma)
	{
		loadModel(path);
	}

	void Draw(Shader &shader)
	{
		for (Mesh mesh : m_meshes)
		{
			mesh.draw(shader);
		}
	}

private:
	// load a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector
	void loadModel(const std::string &path)
	{
		// read file via assimp
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		// check for errors
		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}

		// retrieve the directory path fo the file path
		m_directory = path.substr(0, path.find_last_of('/'));

		// prcess ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}

	// process a node in a recursive fashion. Process each individual mesh located at the node and repeat
	void processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			// the node object only contains indices to index the actual objects in the scene
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes) 
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.emplace_back(processMesh(mesh, scene));
		}

		// after processing all the meshes (if any) recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// data to fill
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;
			// we declare a placeholder vector 
			// since assimp uses its own vector class that doesn't directly convert to glm::vec3 class
			// so we transfer the data to this placeholder glm::vec3 first
			glm::vec3 placeholder3;
			
			// position
			placeholder3.x = mesh->mVertices[i].x;
			placeholder3.y = mesh->mVertices[i].y;
			placeholder3.z = mesh->mVertices[i].z;
			vertex.position = placeholder3;

			// normal
			placeholder3.x = mesh->mNormals[i].x;
			placeholder3.y = mesh->mNormals[i].y;
			placeholder3.z = mesh->mNormals[i].z;
			vertex.normal = placeholder3;

			// texture coords, need to check if the model has the textures or not
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 placeholder2;
				placeholder2.x = mesh->mTextureCoords[0][i].x;
				placeholder2.y = mesh->mTextureCoords[0][i].y;

				vertex.texCoords = placeholder2;
			}
			else
			{
				vertex.texCoords = glm::vec2(0.0f);
			}

			// tangent
			placeholder3.x = mesh->mTangents[i].x;
			placeholder3.y = mesh->mTangents[i].y;
			placeholder3.z = mesh->mTangents[i].z;
			vertex.tangent = placeholder3;

			// bitangent
			placeholder3.x = mesh->mBitangents[i].x;
			placeholder3.y = mesh->mBitangents[i].y;
			placeholder3.z = mesh->mBitangents[i].z;
			vertex.bitangent = placeholder3;
		}

		// now walk through each of the mesh's faces (for indices)
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				indices.emplace_back(face.mIndices[j]);
			}
		}

		// process texture/materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders
		// each diffuse textures should be named as 'texture_diffuseN' 
		//     where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER
		// same applied to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		return Mesh(vertices, indices, textures);
	}

	std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		// TODO
	}
};

#endif // !MODEL_H
