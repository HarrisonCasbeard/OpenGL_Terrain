#include "Mesh.h"
#include <glew\glew.h>

#include "Shader.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
{
	m_vVertices = vertices;
	m_vIndices  = indices;
	m_vTextures = textures;

	setupMesh();
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	//glDeleteVertexArrays(1, &m_VAO);
}

//-------------------------------------------------------------------------------------------------
void Mesh::setupMesh()
{
	//Generate the vertex array and buffers
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vVertices.size() * sizeof(Vertex), &m_vVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndices.size() * sizeof(unsigned int), &m_vIndices[0], GL_STATIC_DRAW);

	//Vert positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Vert Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	//Vert UV's
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);

	glBindVertexArray(m_VAO);

}
//-------------------------------------------------------------------------------------------------
void Mesh::draw(Shader& shader)
{
	unsigned int diffuseCounter = 1;
	unsigned int specularCounter = 1;

	for (unsigned int i = 0; i < m_vTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;

		if (m_vTextures[i].type == "textureDiffuse")
			number = std::to_string(diffuseCounter++);

		else if (m_vTextures[i].type == "textureSpecular")
			number = std::to_string(specularCounter++);

		shader.setFloat("material." + m_vTextures[i].type + number, (float)i);
		glBindTexture(GL_TEXTURE_2D, m_vTextures[i].id);
	}

	//Draw the mesh
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_vIndices.size(), GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}