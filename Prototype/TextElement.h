#ifndef _TEXT_ELEMENT_H_
#define _TEXT_ELEMENT_H_

#include <glm\common.hpp>
#include <string>
#include <unordered_map>
#include <glew\glew.h>
#include <glm\mat4x4.hpp>

//-------------------------------------------------------------------------------------------------
struct Character
{
	GLuint		textureID;
	glm::ivec2	size;
	glm::ivec2	bearing;
	GLuint		advance;
};
typedef std::unordered_map<GLchar, Character> Font;
//-------------------------------------------------------------------------------------------------
class Shader;

class TextElement
{
private:
	glm::vec2	m_position;
	glm::vec3	m_colour;
	glm::vec2	m_scale;
	std::string m_textStr;
	bool		m_isVisible;
	const Font*	m_pFont;

public:
	 TextElement(const glm::vec2& pos, const std::string& text, const Font& font,
				 const bool isVisible = false, const glm::vec3& colour = { 1.0f, 0.0f, 0.0f }, const glm::vec2& scale = { 1.0f, 1.0f });
	 TextElement();
	~TextElement();

	void initialise(const glm::vec2& pos, const std::string& text, const Font& font,
					const bool isVisible = false, const glm::vec3& colour = { 1.0f, 0.0f, 0.0f }, const glm::vec2& scale = { 1.0f, 1.0f });
	void draw(const GLuint shaderID, const GLuint VBO);

	//Grim stuff
	void			 position(const glm::vec2& newPos);
	const glm::vec2& position() const;

	void			 colour(const glm::vec3& newCol);
	const glm::vec3& colour() const;

	void			 scale(const glm::vec2& newScale);
	const glm::vec2& scale() const;

	void				text(const std::string& newStr);
	const std::string&	text() const;

	void		isVisible(const bool isVisible);
	const bool	isVisible() const;

};

#endif // !

