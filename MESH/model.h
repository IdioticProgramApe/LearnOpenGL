#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <vector>

#include "shader.h"
#include "mesh.h"
#include "image.hpp"

class Model
{
public:
	Model(char *path) { m_loadModel(path); }
	~Model() {};
	void Draw(Shader shader);

private:
	std::vector<Texture> m_loadedTextures;
	std::vector<Mesh> m_meshes;
	std::string m_directory;

	void m_loadModel(char *path);
	void m_processNode(aiNode *node, const aiScene *scene);
	Mesh m_processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> m_loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif // !MODEL_H
