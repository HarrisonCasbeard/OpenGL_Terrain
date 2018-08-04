#include "Water.h"
#include <glew\glew.h>
#include <glm\matrix.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Shader.h"

Water::Water(Shader& shader, const glm::vec3& pos, const glm::vec3& scale)
{
	initialise(shader, pos, scale);
}

Water::~Water()
{
	glDeleteVertexArrays(1, &m_VAO);
}

//-------------------------------------------------------------------------------------------------
void Water::initialise(Shader& shader, const glm::vec3& pos, const glm::vec3& scale)
{
	m_position = pos;
	m_scale = scale;
	m_pShader = &shader;

	setupBuffers();
}
//-------------------------------------------------------------------------------------------------
void Water::render()
{
	if (!m_pShader)
	{
		printf("Water has no shader!! Skipping rendering... \n");
		return;
	}

	m_pShader->activate();

	glm::mat4 matrix(1.0f);
	matrix = glm::translate(matrix, m_position);
	matrix = glm::scale(matrix, m_scale);
	m_pShader->setMatrixf("model", &matrix[0][0]);

	glBindTexture(GL_TEXTURE_2D, m_texID);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
//-------------------------------------------------------------------------------------------------
void Water::setupBuffers()
{
	//Verts for quad
	float verts[] =
	{
		-1.0f, 0.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,
		 1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	glGenTextures(1, &m_texID);
	glBindTexture(GL_TEXTURE_2D, m_texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindTexture(GL_TEXTURE_2D, 0);
}