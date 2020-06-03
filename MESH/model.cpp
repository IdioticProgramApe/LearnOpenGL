#include "model.h"

void Model::Draw(Shader shader)
{
	for (auto mesh : m_meshes) mesh.Draw(shader);
}

void Model::m_loadModel(char * path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	std::string stringPath = (std::string)path;
	m_directory = stringPath.substr(0, stringPath.find_last_of('/'));

	m_processNode(scene->mRootNode, scene);
}

void Model::m_processNode(aiNode * node, const aiScene * scene)
{
	// process parent nodes
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.emplace_back(mesh);
	}

	// process children nodes
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		m_processNode(node->mChildren[i], scene);
	}
}

Mesh Model::m_processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		
		// process the mesh vertex data and store it to the vertex
		// position
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		// normal
		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		// texture coords
		if (mesh->mTextureCoords)
		{
			vertex.texCoords.x = mesh->mTextureCoords[i]->x;
			vertex.texCoords.y = mesh->mTextureCoords[i]->y;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f);
		}
		

		vertices.emplace_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.emplace_back(face.mIndices[j]);
		}
	}

	// process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMap = m_loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMap.begin(), diffuseMap.end());

		std::vector<Texture> specularMap = m_loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMap.begin(), specularMap.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::m_loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip{ false };
		for (unsigned int j = 0; j < m_loadedTextures.size(); ++j)
		{
			if (std::strcmp(str.C_Str(), m_loadedTextures[j].path.C_Str()) == 0)
			{
				textures.emplace_back(m_loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), m_directory).ID;
			texture.type = typeName;
			texture.path = str;
			textures.emplace_back(texture);
			m_loadedTextures.emplace_back(texture);
		}
	}
	return textures;
}
