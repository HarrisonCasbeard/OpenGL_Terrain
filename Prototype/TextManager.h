#ifndef _TEXT_MANAGER_H_
#define _TEXT_MANAGER_H_

#include <ft2build.h>
#include <freetype\freetype.h>
#include <glm\mat4x4.hpp>

#include "TextElement.h"

class Shader;

class TextManager
{
private:
	void loadFonts(const std::unordered_map<std::string, std::string>& fonts);
	void createFontTextures(FT_Face& face, Font& font);
	void generateBuffers();

	std::unordered_map<std::string, Font>		 m_mFonts;
	std::unordered_map<std::string, TextElement> m_mTextElements;

	GLuint		m_textVAO;
	GLuint		m_textVBO;
	glm::mat4	m_projMatrix;

public:
	 TextManager();
	~TextManager();

	void initialise(const float screenW, const float screenH);
	void render(Shader& shader);

	void setElementString(const std::string& id, const std::string& newStr);
	void setElementVisible(const std::string& id, const bool isVisible);
};

#endif
