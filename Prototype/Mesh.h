#ifndef _MESH_H_
#define _MESH_H_

#include <vector> //I'm dubious of using a vector for this
#include <string>
#include <glm\glm.hpp>
#include <assimp\types.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct Texture
{
	unsigned int id;
	std::string	 type;
	aiString	 path;
};

class Shader;

class Mesh
{
private:

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

public:
	 Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
	 Mesh();
	~Mesh();

	void setupMesh	();
	void draw		(Shader& shader);

	std::vector<Vertex>			m_vVertices;
	std::vector<unsigned int>	m_vIndices;
	std::vector<Texture>		m_vTextures;

};

#endif
