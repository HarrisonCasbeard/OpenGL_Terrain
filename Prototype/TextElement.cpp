#include "TextElement.h"

TextElement::TextElement(const glm::vec2& pos, const std::string& text, const Font& font, const bool isVisible, const glm::vec3& colour, const glm::vec2& scale)
{
	initialise(pos, text, font, isVisible, colour, scale);
}

TextElement::TextElement()
{
}

TextElement::~TextElement()
{
}
//-------------------------------------------------------------------------------------------------
void TextElement::initialise(const glm::vec2& pos, const std::string& text, const Font& font, const bool isVisible, const glm::vec3& colour, const glm::vec2& scale)
{
	m_position = pos;
	m_textStr = text;
	m_pFont = &font;
	m_isVisible = isVisible;
	m_colour = colour;
	m_scale = scale;
}
//-------------------------------------------------------------------------------------------------
void TextElement::draw(const GLuint shaderID, const GLuint VBO)
{
	if (m_isVisible)
	{
		glUniform3f(glGetUniformLocation(shaderID, "textColour"), m_colour.x, m_colour.y, m_colour.z);

		GLfloat curX = m_position.x;
		GLfloat curY = m_position.y;

		auto itr = m_textStr.begin();
		for (; itr != m_textStr.end(); itr++)
		{
			//Find the character in the font
			Character c = m_pFont->at(*itr);

			//Find it's pos and dimensions for vert calculations
			GLfloat xpos = curX + (c.bearing.x * m_scale.x);
			GLfloat ypos = curY - (c.size.y - c.bearing.y) * m_scale.y;

			GLfloat width = c.size.x * m_scale.x;
			GLfloat height = c.size.y * m_scale.y;

			//ypos = bottom left!
			GLfloat vertices[6][4] =
			{
				{ xpos,			ypos + height,	0.0f, 0.0f },
				{ xpos,			ypos,			0.0f, 1.0f },
				{ xpos + width, ypos,			1.0f, 1.0f },

				{ xpos,			ypos + height,	0.0f, 0.0f },
				{ xpos + width, ypos,			1.0f, 1.0f },
				{ xpos + width, ypos + height,	1.0f, 0.0f },
			};

			//Render the glpyh over the defined quad
			glBindTexture(GL_TEXTURE_2D, c.textureID);
			//Add the data
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			//Render the quad
			glDrawArrays(GL_TRIANGLES, 0, 6);

			//Advance the cursor along the X for the next letter
			curX += (c.advance >> 6) * m_scale.x;
		}
	}
}
//-------------------------------------------------------------------------------------------------
void TextElement::position(const glm::vec2& newPos)
{
	m_position = newPos;
}
//-------------------------------------------------------------------------------------------------
const glm::vec2& TextElement::position() const
{
	return m_position;
}
//-------------------------------------------------------------------------------------------------
void TextElement::colour(const glm::vec3& newCol)
{
	m_colour = newCol;
}
//-------------------------------------------------------------------------------------------------
const glm::vec3& TextElement::colour() const
{
	return m_colour;
}
//-------------------------------------------------------------------------------------------------
void TextElement::scale(const glm::vec2& newScale)
{
	m_scale = newScale;
}
//-------------------------------------------------------------------------------------------------
const glm::vec2& TextElement::scale() const
{
	return m_scale;
}
//-------------------------------------------------------------------------------------------------
void TextElement::text(const std::string& newStr)
{
	m_textStr = newStr;
}
//-------------------------------------------------------------------------------------------------
const std::string& TextElement::text() const
{
	return m_textStr;
}
//-------------------------------------------------------------------------------------------------
void TextElement::isVisible(const bool isVisible)
{
	m_isVisible = isVisible;
}
//-------------------------------------------------------------------------------------------------
const bool TextElement::isVisible() const
{
	return m_isVisible;
}