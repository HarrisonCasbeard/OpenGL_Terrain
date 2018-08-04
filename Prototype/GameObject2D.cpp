#include "GameObject2D.h"
#include "Shader.h"
#include <glm\gtc\matrix_transform.hpp>
#include "stb_image.h"

GameObject2D::GameObject2D(const std::string& filepath, const glm::vec2& pos, Shader& shader, const float rotInDeg, const glm::vec2& scale, const glm::vec3& colour)
{
	initialise(filepath, pos, shader, rotInDeg, scale, colour);
}

GameObject2D::~GameObject2D()
{
	glDeleteVertexArrays(1, &m_VAO);
}

//-------------------------------------------------------------------------------------------------
void GameObject2D::initialise(const std::string& filepath, const glm::vec2& pos, Shader& shader, const float rotInDeg, const glm::vec2& scale, const glm::vec3& colour)
{
	initBuffers();
	initTexture(filepath);

	m_is3D		= false;

	m_position	= pos;
	m_rotation	= rotInDeg;
	m_colour	= colour;
	m_scale		= scale;
	m_pShader = &shader;

	calcMatrix();
}
//-------------------------------------------------------------------------------------------------
void GameObject2D::render()
{
	if (!m_pShader)
	{
		printf("Object has no shader!!! Skipping rendering! \n");
		return;
	}

	m_pShader->activate();

	m_pShader->setMatrixf("model", &m_modelMatrix[0][0]);
	m_pShader->setVec3("spriteColour", m_colour);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texID);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
//-------------------------------------------------------------------------------------------------
void GameObject2D::initBuffers()
{
	GLfloat vertices[] =
	{
		//Pos		//Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
//-------------------------------------------------------------------------------------------------
void GameObject2D::initTexture(const std::string& filepath)
{
	glGenTextures(1, &m_texID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		m_dimensions = { width, height };

		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//When MAGnifying the image (no bigger mipmap available) use . . .
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}
	else
	{
		printf("FAILED TO LOAD TEXTURE : %s \n", filepath.c_str());
		stbi_image_free(data);
	};
}
//-------------------------------------------------------------------------------------------------
void GameObject2D::calcMatrix()
{
	m_modelMatrix = glm::mat4(1.0f);

	//Translate the model
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(m_position, 0.0f));

	//Then rotate
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3((m_dimensions * 0.5f) * m_scale, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3((m_dimensions * -0.5f) * m_scale, 0.0f));

	//Then scale
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_dimensions * m_scale, 1.0f));
}
//-------------------------------------------------------------------------------------------------
void GameObject2D::position(const glm::vec2& newPos)
{
	m_position = newPos;
	calcMatrix();
}
//-------------------------------------------------------------------------------------------------
const glm::vec2& GameObject2D::position() const
{
	return m_position;
}
//-------------------------------------------------------------------------------------------------
void GameObject2D::rotation(const float rotInDeg)
{
	m_rotation = rotInDeg;
	calcMatrix();
}
//-------------------------------------------------------------------------------------------------
const float GameObject2D::rotation() const
{
	return m_rotation;
}
//-------------------------------------------------------------------------------------------------
void GameObject2D::scale(const glm::vec2& newScale)
{
	m_scale = newScale;
	calcMatrix();
}
//-------------------------------------------------------------------------------------------------
const glm::vec2& GameObject2D::scale() const
{
	return m_scale;
}
//-------------------------------------------------------------------------------------------------
void GameObject2D::colour(const glm::vec3& newCol)
{
	m_colour = newCol;
}
//-------------------------------------------------------------------------------------------------
const glm::vec3& GameObject2D::colour() const
{
	return m_colour;
}
//-------------------------------------------------------------------------------------------------
void GameObject2D::incPosition(const glm::vec2& delta)
{
	m_position += delta;
	calcMatrix();
}
//-------------------------------------------------------------------------------------------------