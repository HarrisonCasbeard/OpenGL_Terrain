#include "Terrain.h"
#include <istream>
#include <fstream> 
#include "stb_image.h"
#include <glm\gtx\compatibility.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Shader.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

//-------------------------------------------------------------------------------------------------
inline float getPercentage(float value, const float min, const float max)
{
	value = glm::clamp(value, min, max);
	return (value - min) / (max - min);
}
//-------------------------------------------------------------------------------------------------
inline int getFileLength(std::istream& file)
{
	//Store the current position
	int pos = (int)file.tellg();

	//Find the end
	file.seekg(0, std::ios::end);
	int length = (int)file.tellg();

	//Restore initial pointer position
	file.seekg(pos);

	return length;
}
//-------------------------------------------------------------------------------------------------
inline void deleteVertexBuffer(GLuint& vboID)
{
	if (vboID)
	{
		glDeleteBuffersARB(1, &vboID);
		vboID = 0;
	}
}
//-------------------------------------------------------------------------------------------------
inline void createVertexBuffer(GLuint& vboID)
{
	deleteVertexBuffer(vboID);
	glGenBuffersARB(1, &vboID);
}
//-------------------------------------------------------------------------------------------------
inline void deleteTexture(GLuint& texID)
{
	if (texID)
	{
		glDeleteTextures(1, &texID);
		texID = 0;
	}
}
//-------------------------------------------------------------------------------------------------
inline void createTexture(GLuint& texID)
{
	deleteTexture(texID);
	glGenTextures(1, &texID);
}
//-------------------------------------------------------------------------------------------------
inline float getHeightValue(const unsigned char* data, unsigned char numBytes)
{
	switch (numBytes)
	{
		case 1:
		{
			return (unsigned char)(data[0]) / (float)0xff;
			break;
		}
		case 2:
		{
			return (unsigned short)(data[1] << 8 | data[0]) / (float)0xffff;
			break;
		}
		case 4:
		{
			return (unsigned int)(data[3] << 24 | data[2]) / (float)0xffffff;
			break;
		}
		default:
		{
			assert(false);
			break;
		}
	}

	return 0.0f;
}
//-------------------------------------------------------------------------------------------------
Terrain::Terrain(Shader& shader, const float heightScale, const float blockSize)
{
	initialise(shader, heightScale, blockSize);
}

Terrain::~Terrain()
{
	terminate();
}
//-------------------------------------------------------------------------------------------------
void Terrain::initialise(Shader& shader, const float heightScale, const float blockSize)
{
	m_heightScale = heightScale;
	m_blockScale = blockSize;

	m_localToWorldMatrix = glm::mat4(1.0f);
	m_pShader = &shader;
}
//-------------------------------------------------------------------------------------------------
void Terrain::terminate()
{
	deleteVertexBuffer(m_VBO);
	deleteVertexBuffer(m_indexVBO);
	glDeleteVertexArrays(1, &m_VAO);

	for (unsigned int i = 0; i < m_numOfTextures; i++)
	{
		deleteTexture(m_textures[i]);
	}
}
//-------------------------------------------------------------------------------------------------
void Terrain::render()
{
	m_pShader->activate();
	m_pShader->setMatrixf("model", &glm::mat4(1.0f)[0][0]);

	m_pShader->setFloat("maxHeight", m_heightScale);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textures[1]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_textures[2]);

	if (m_shadowMap)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_shadowMap);
	}

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_vIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
//-------------------------------------------------------------------------------------------------
void Terrain::shadowRender()
{
	m_pShader->activate();
	m_pShader->setMatrixf("model", &glm::mat4(1.0f)[0][0]);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_vIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
//-------------------------------------------------------------------------------------------------
bool Terrain::loadTexture(const std::string& filepath, unsigned int textureStage)
{
	assert(textureStage < m_numOfTextures);
	deleteTexture(m_textures[textureStage]);

	int width, height, components;

	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &components, 0);
	if (data)
	{
		GLenum format;
		if (components == 1)
			format = GL_RED;
		else if (components == 3)
			format = GL_RGB;
		else if (components == 4)
			format = GL_RGBA;

		glGenTextures(1, &m_textures[textureStage]);
		glBindTexture(GL_TEXTURE_2D, m_textures[textureStage]);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
		return true;
	}

	stbi_image_free(data);
	return false;
}
//-------------------------------------------------------------------------------------------------
bool Terrain::loadHeightMap(const std::string& filepath, unsigned char bitsPerPixel, unsigned int width, unsigned int height)
{
	std::ifstream ifs;
	ifs.open(filepath, std::ifstream::binary);
	if (ifs.fail())
	{
		printf("FAILED TO OPEN FILE : %s", filepath.c_str());
		ifs.close();
		return false;
	}

	const unsigned int bytesPerPixel = bitsPerPixel / 8;
	const unsigned int expectedFileSize = (bytesPerPixel * width * height);
	const unsigned int fileSize = getFileLength(ifs);

	if (expectedFileSize != fileSize)
	{
		printf("Expected File Size : [ %d ] bytes differs from actual file size [ %d ]", expectedFileSize, fileSize);
		ifs.close();
		return false;
	}

	unsigned char* heightMap = new unsigned char[fileSize];
	ifs.read((char*)heightMap, fileSize);
	if (ifs.fail())
	{
		printf("An error occurred while reading from height map file: %s", filepath.c_str());
		ifs.close();
		delete[] heightMap;
		return false;
	}
	ifs.close();

	//Fully loaded RAW file, so now let's manipulate the data
	unsigned int numVerts = width * height;
	m_vVertices.resize(numVerts);

	m_heightMapDimensions = { width, height };

	//World units from - to size-1
	//However, we want the range to be from -halfWidth to +halfWidth etc
	float terrainWidth = (width - 1) * m_blockScale;
	float terrainHeight = (height - 1) * m_blockScale;

	float halfTerrainWidth = terrainWidth * 0.5f;
	float halfTerrainHeight = terrainHeight * 0.5f;

	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			//Index is better explained as "pixel" - it is the current pixel in the height map
			unsigned int index = (j * width) + i;
			assert(index * bytesPerPixel < fileSize);
			float heightValue = getHeightValue(&heightMap[index * bytesPerPixel], bytesPerPixel);

			//Think of the ENTIRE terrain as 0 to 1, not each triangle
			float s = (i / (float)(width - 1));
			float t = (j / (float)(height - 1));

			float x = (s * terrainWidth) - halfTerrainWidth;
			float y = heightValue * m_heightScale;
			float z = (t * terrainHeight) - halfTerrainHeight;

			//This blends the textures depending on their height
			float tex0Contribution = 1.0f - getPercentage(heightValue,  0.0f, 0.75f);
			float tex2Contribution = 1.0f - getPercentage(heightValue, 0.75f, 1.0f);

			m_vVertices[index].normal = glm::vec3(0);
			m_vVertices[index].position = glm::vec3(x, y, z);
			m_vVertices[index].uv = glm::vec2(s, t);
		}
	}

	printf("Terrain successfully loaded!");
	delete[] heightMap;

	generateIndexBuffer();
	generateNormals();
	generateVertexBuffers();

	return true;
}
//-------------------------------------------------------------------------------------------------
void Terrain::generateIndexBuffer()
{
	if (m_heightMapDimensions.x < 2 || m_heightMapDimensions.y < 2)
	{
		printf("Terrain hasn't been loaded or is an inadequate size!");
		return;
	}

	const unsigned int terrainWidth  = (unsigned int)m_heightMapDimensions.x;
	const unsigned int terrainHeight = (unsigned int)m_heightMapDimensions.y;

	//2 triangles for every quad
	const unsigned int numTris = (terrainWidth - 1) * (terrainHeight - 1) * 2;

	//3 indices for each triangle in the terrain mesh
	m_vIndices.resize(numTris * 3);

	//index in the index buffer
	unsigned int index = 0;
	for (unsigned int j = 0; j < (terrainHeight - 1); ++j)
	{
		for (unsigned int i = 0; i < (terrainWidth - 1); ++i)
		{
			int vertexIndex = (j * terrainWidth) + i;
			//Right or "top" triangle
			m_vIndices[index++] = vertexIndex;						//V0
			m_vIndices[index++] = vertexIndex + terrainWidth + 1;	//V3
			m_vIndices[index++] = vertexIndex + 1;					//V1
			//Left or "bottom triangle
			m_vIndices[index++] = vertexIndex;						//V0
			m_vIndices[index++] = vertexIndex + terrainWidth;		//V2
			m_vIndices[index++] = vertexIndex + terrainWidth + 1;	//V3
		}
	}
}
//-------------------------------------------------------------------------------------------------
void Terrain::generateNormals()
{
	for (unsigned int i = 0; i < m_vIndices.size(); i += 3)
	{
		glm::vec3 v0 = m_vVertices[m_vIndices[i + 0]].position;
		glm::vec3 v1 = m_vVertices[m_vIndices[i + 1]].position;
		glm::vec3 v2 = m_vVertices[m_vIndices[i + 2]].position;

		glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		m_vVertices[m_vIndices[i + 0]].normal += normal;
		m_vVertices[m_vIndices[i + 1]].normal += normal;
		m_vVertices[m_vIndices[i + 2]].normal += normal;
	}

	const glm::vec3 UP(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_vVertices.size(); i++)
	{
		m_vVertices[i].normal = glm::normalize(m_vVertices[i].normal);
	}
}
//-------------------------------------------------------------------------------------------------
void Terrain::generateVertexBuffers()
{
	//Generate all ID's
	glGenVertexArrays(1, &m_VAO);
	glGenBuffersARB(1, &m_VBO);
	glGenBuffersARB(1, &m_indexVBO);

	glBindVertexArray(m_VAO);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_VBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_vVertices.size() * sizeof(Vertex), &m_vVertices[0], GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_indexVBO);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_vIndices.size() * sizeof(GLuint), &m_vIndices[0], GL_STATIC_DRAW_ARB);

	//Vert positions
	glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArrayARB(0);
	//Vert normals
	glVertexAttribPointerARB(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArrayARB(1);
	//Vert UV's
	glVertexAttribPointerARB(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArrayARB(2);

	glBindVertexArray(0);
}
//-------------------------------------------------------------------------------------------------
float Terrain::getHeightAt(const glm::vec3& pos)
{
	float height = -(FLT_MAX);
	if (m_heightMapDimensions.x < 2 || m_heightMapDimensions.y < 2)
		return height;

	//Width and height in world units
	float terrainWidth  = (m_heightMapDimensions.x - 1) * m_blockScale;
	float terrainHeight = (m_heightMapDimensions.y - 1) * m_blockScale;
	float halfWidth = terrainWidth * 0.5f;
	float halfHeight = terrainHeight * 0.5f;

	//Multiply the position by the inverse of the terrain matrix
	//This returns the position in local terrain space
	glm::mat4 invMatrix = glm::inverse(m_localToWorldMatrix);
	glm::vec3 localTerrainPos = glm::vec3(invMatrix * glm::vec4(pos, 1.0f));
	glm::vec3 invBlockScale(1.0f / m_blockScale, 0.0f, 1.0f / m_blockScale);

	//Calc an offset and scale to get the vertex indices
	//Offset due to 0,0 being the middle
	glm::vec3 offset(halfWidth, 0.0f, halfHeight);

	//Get the 4 verts that make the quad we're on
	glm::vec3 vertexIndices = (localTerrainPos + offset) * invBlockScale;

	int u0 = (int)floorf(vertexIndices.x);
	int u1 = u0 + 1;
	int v0 = (int)floorf(vertexIndices.z);
	int v1 = v0 + 1;

	if (u0 >= 0 && u1 < m_heightMapDimensions.x && v0 >= 0 && v1 < m_heightMapDimensions.y)
	{
		glm::vec3 p00 = m_vVertices[(v0 * m_heightMapDimensions.x) + u0].position;
		glm::vec3 p10 = m_vVertices[(v0 * m_heightMapDimensions.x) + u1].position;
		glm::vec3 p01 = m_vVertices[(v1 * m_heightMapDimensions.x) + u0].position;
		glm::vec3 p11 = m_vVertices[(v1 * m_heightMapDimensions.x) + u1].position;

		//Which triangle are we over?
		float percentU = vertexIndices.x - u0;
		float percentV = vertexIndices.z - v0;

		glm::vec3 dU, dV;
		if (percentU > percentV)
		{
			//Right "top" triangle
			dU = p10 - p00;
			dV = p11 - p10;
		}
		else
		{
			//Left "bottom" triangle
			dU = p11 - p01;
			dV = p01 - p00;
		}

		glm::vec3 heightPos = p00 + (dU * percentU) + (dV * percentV);
		//Convert back to world-space by multiplying by the terrain matrix
		heightPos = glm::vec3(m_localToWorldMatrix * glm::vec4(heightPos, 1.0f));
		height = heightPos.y;
	}

	printf("Height on terrain : %f \n", height);
	return height;
}
//-------------------------------------------------------------------------------------------------
void Terrain::setShadowMap(const GLuint shadowMap)
{
	m_shadowMap = shadowMap;
}