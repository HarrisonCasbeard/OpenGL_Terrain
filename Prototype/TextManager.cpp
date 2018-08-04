#include "TextManager.h"
#include <vector>
#include <glm\gtc\matrix_transform.hpp>
#include "Shader.h"

TextManager::TextManager()
{
}

TextManager::~TextManager()
{
}

//-------------------------------------------------------------------------------------------------
void TextManager::initialise(const float screenW, const float screenH)
{
	m_projMatrix = glm::ortho(0.0f, screenW, 0.0f, screenH);

	//----- Fonts -----
	std::unordered_map<std::string, std::string> fonts =
	{
		//ID			//FilePath
		{ "DejaVuSans", "Assets/Fonts/DejaVuSans.ttf" },
	};
	loadFonts(fonts);
	
	//----- Elements -----
	m_mTextElements =
	{
		{ "Time",	TextElement({  30.0f, 875.0f }, "time : ",		m_mFonts["DejaVuSans"], true) },
		{ "Score",	TextElement({ 900.0f, 875.0f }, "score : ",		m_mFonts["DejaVuSans"], true) },
	};
}
//-------------------------------------------------------------------------------------------------
void TextManager::render(Shader& shader)
{
	//Use the text shader
	shader.activate();
	shader.setMatrixf("projection", &m_projMatrix[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_textVAO);

	auto itr = m_mTextElements.begin();
	for (; itr != m_mTextElements.end(); itr++)
	{
		itr->second.draw(shader.m_shaderID, m_textVBO);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
//-------------------------------------------------------------------------------------------------
void TextManager::setElementString(const std::string& id, const std::string& newStr)
{
	auto itr = m_mTextElements.find(id);
	if (itr != m_mTextElements.end())
	{
		itr->second.text(newStr);
	}
	else
	{
		printf("TEXT ELEMENT '%s' DOESN'T EXIST!", id.c_str());
	}
}
//-------------------------------------------------------------------------------------------------
void TextManager::setElementVisible(const std::string& id, const bool isVisible)
{
	auto itr = m_mTextElements.find(id);
	if (itr != m_mTextElements.end())
	{
		itr->second.isVisible(isVisible);
	}
	else
	{
		printf("TEXT ELEMENT '%s' DOESN'T EXIST!", id.c_str());
	}
}

//***** TO DO :  MAKE A FONT MANAGER TO HANDLE THIS SHIT *****
//-------------------------------------------------------------------------------------------------
void TextManager::loadFonts(const std::unordered_map<std::string, std::string>& fonts)
{
	//Load FreeType
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		printf("Could not init freetype library\n");
		return;
	}

	//Loop through all vector and load all fonts
	FT_Face face = nullptr;
	auto itr = fonts.begin();
	for (; itr != fonts.end(); itr++)
	{
		//Load Font - skip if couldn't be loaded
		if (FT_New_Face(ft, itr->second.c_str(), 0, &face))
		{
			printf("COULDN'T LOAD FONT : %s - Skipping", itr->second.c_str());
			continue;
		}
		FT_Set_Pixel_Sizes(face, 0, 48);

		m_mFonts.insert({ itr->first,{} });
		createFontTextures(face, m_mFonts[itr->first]);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	generateBuffers();
}
//-------------------------------------------------------------------------------------------------
void TextManager::createFontTextures(FT_Face& face, Font& font)
{
	//Due to Greyscale, it's not 4 bytes, it's just 1!
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Loop through all ASCII Characters
	for (GLubyte c = 0; c < 128; c++)
	{
		//Find the ASCII character in font
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			printf("FAILED TO LOAD %c IN FONT! - Skipping", c);
			continue;
		}

		//To save constant code calling
		FT_GlyphSlot glyph = face->glyph;

		//Create the Texture
		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph->bitmap.width, glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Store all the required info into the structure
		Character character;
		character =
		{
			texID,
			{ glyph->bitmap.width, glyph->bitmap.rows },
			{ glyph->bitmap_left, glyph->bitmap_top },
			(GLuint)glyph->advance.x
		};
		
		font.insert({ c, character });
	}
}
//-------------------------------------------------------------------------------------------------
void TextManager::generateBuffers()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_textVAO);
	glGenBuffers(1, &m_textVBO);

	glBindVertexArray(m_textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}