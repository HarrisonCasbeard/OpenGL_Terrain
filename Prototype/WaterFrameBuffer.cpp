#include "WaterFrameBuffer.h"

WaterFrameBuffer::WaterFrameBuffer()
{
	initFrameBuffer();
}

WaterFrameBuffer::~WaterFrameBuffer()
{
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteTextures(1, &m_texID);
}

//-------------------------------------------------------------------------------------------------
void WaterFrameBuffer::initFrameBuffer()
{
	glGenFramebuffers(1, &m_FBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}
//-------------------------------------------------------------------------------------------------
void WaterFrameBuffer::bind(const int width, const int height)
{
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}
//-------------------------------------------------------------------------------------------------
void WaterFrameBuffer::unbind(const int width, const int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//----- Reset Viewport -----
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_TEST);
}
//-------------------------------------------------------------------------------------------------
void WaterFrameBuffer::addColourTextureAttachment(const int width, const int height)
{
	glGenTextures(1, &m_texID);
	glBindTexture(GL_TEXTURE_2D, m_texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//-------------------------------------------------------------------------------------------------
void WaterFrameBuffer::addDepthTextureAttachment(const int width, const int height)
{
	glGenTextures(1, &m_texID);
	glBindTexture(GL_TEXTURE_2D, m_texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//-------------------------------------------------------------------------------------------------
void WaterFrameBuffer::addDepthBufferAttachment(const int width, const int height)
{
	glGenRenderbuffers(1, &m_renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, m_renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FAILED TO CREATE DEPTH BUFFER ATTACHMENT!! \n");
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}