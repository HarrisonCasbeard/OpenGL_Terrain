#ifndef _MODEL_H_
#define _MODEL_H_

#include "Mesh.h"
#include <assimp\scene.h>

class Shader;	//Decoupling some mofo's

class Model
{
private:
	bool loadModel	 (const std::string& path);
	void processNode (const aiNode& node, const aiScene& scene);
	Mesh processMesh (const aiMesh& mesh, const aiScene& scene);
	void findDimensions();

	void loadMaterialTextures(const aiMaterial& mat, aiTextureType texType, const std::string& typeName,std::vector<Texture>& textures);
	unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false);

	std::vector<Mesh>	 m_vMeshes;
	std::string			 m_directory;
	std::vector<Texture> m_vLoadedTextures;

	glm::vec3 m_dimensions;

public:
	 Model(const char* path);
	 Model();
	 ~Model();

	void draw(Shader& shader);

	const glm::vec3& dimensions	() const;
};

#endif