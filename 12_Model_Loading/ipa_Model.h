#ifndef MODEL_H
#define MODEL_H

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm.hpp"

#include <string>
#include <iostream>
#include <vector>

#include "ipa_Shader.h"
#include "ipa_Mesh.h"
#include "ipa_Image.h"

class Model
{
public:
	Model(const char *path) { loadModel(path); }
	~Model() {}
	void draw(Shader shader);

private:
	std::vector<Texture> m_loadedTextures;
	std::vector<Mesh> m_meshes;
	std::string m_directory;

	void loadModel(const char *path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif // !MODEL_H
