#include "ShadowMap.h"

ShadowMap::ShadowMap(const glm::ivec2& res)
{
	m_resolution = res;
}

ShadowMap::~ShadowMap()
{
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteTextures(1, &m_texID);
}

//-------------------------------------------------------------------------------------------------
void ShadowMap::initialise()
{

	glGenTextures(1, &m_texID);
	glBindTexture(GL_TEXTURE_2D, m_texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (GLsizei)m_resolution.x, (GLsizei)m_resolution.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//-------------------------------------------------------------------------------------------------
void ShadowMap::bind()
{
	glViewport(0, 0, m_resolution.x, m_resolution.y);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}
//-------------------------------------------------------------------------------------------------
void ShadowMap::unbind(const glm::ivec2& screenDimensions)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//----- Reset Viewport -----
	glViewport(0, 0, screenDimensions.x, screenDimensions.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_TEST);
}
//-------------------------------------------------------------------------------------------------
const GLuint ShadowMap::textureID() const
{
	return m_texID;
}