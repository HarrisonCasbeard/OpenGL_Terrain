#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "Drawable.h"
#include <string>
#include <glm\glm.hpp>
#include <vector>
#include <glew\glew.h>

class Shader;

class Terrain : public Drawable
{
private:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	std::vector<Vertex> m_vVertices;
	std::vector<GLuint> m_vIndices;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_indexVBO;
	GLuint m_shadowMap;

	static const unsigned int m_numOfTextures = 3;
	GLuint m_textures[m_numOfTextures];

	glm::mat4 m_localToWorldMatrix;
	glm::vec2 m_heightMapDimensions;

	float m_heightScale;
	float m_blockScale;	

protected:
	void generateIndexBuffer	();
	void generateNormals		();
	void generateVertexBuffers	();

public:
			 Terrain(Shader& shader, const float heightScale = 500.0f, const float blockSize = 2.0f);
	virtual ~Terrain();

	void initialise(Shader& shader, const float heightScale = 500.0f, const float blockSize = 2.0f);
	void terminate();

	void setShadowMap(const GLuint shadowMap);

	bool loadHeightMap	(const std::string& filepath, unsigned char bitsPerPixel, unsigned int width, unsigned int height);
	bool loadTexture	(const std::string& filepath, unsigned int textureStage = 0);

	float getHeightAt(const glm::vec3& pos);

	virtual void render		 () override;
			void shadowRender();
};

#endif

