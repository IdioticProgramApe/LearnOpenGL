#include "model.h"

void Model::Draw(Shader shader)
{
	for (auto mesh : m_meshes)
	{
		mesh.Draw(shader);
	}
}

void Model::m_loadModel(char * path)
{
}

void Model::m_processNode(aiNode * node, const aiScene * scene)
{
}

Mesh Model::m_processMesh(aiMesh * mesh, const aiScene * scene)
{
	return Mesh();
}

std::vector<Texture> Model::m_loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	return std::vector<Texture>();
}
