#include "FrameBuffer.h"
#include <glew\glew.h>
#include <stdio.h>

FrameBuffer::FrameBuffer(const int screenW, const int screenH)
{
	m_isEnabled = true;

	createFrameBuffers(screenW, screenH);
	createQuadBuffers();
}

FrameBuffer::~FrameBuffer()
{
}

//-------------------------------------------------------------------------------------------------
void FrameBuffer::createFrameBuffers(const int screenW, const int screenH)
{
	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	glGenTextures(1, &m_textureBuffer);
	glBindTexture(GL_TEXTURE_2D, m_textureBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenW, screenH, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureBuffer, 0);

	glGenRenderbuffers(1, &m_renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenW, screenH);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FAILED TO CREATE FRAME BUFFER!! \n");
		m_isEnabled = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//-------------------------------------------------------------------------------------------------
void FrameBuffer::createQuadBuffers()
{
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
							 // positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_quadVAO);
	glBindVertexArray(m_quadVAO);

	glGenBuffers(1, &m_quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));
}
//-------------------------------------------------------------------------------------------------
void FrameBuffer::draw()
{
	//Bind back to original frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	//Clear colours on the 'new' buffer
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(m_quadVAO);
	glBindTexture(GL_TEXTURE_2D, m_textureBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
//-------------------------------------------------------------------------------------------------
void FrameBuffer::enabled(const bool isEnabled)
{
	m_isEnabled = isEnabled;
}
//-------------------------------------------------------------------------------------------------
const bool FrameBuffer::enabled() const
{
	return m_isEnabled;
}
//-------------------------------------------------------------------------------------------------
const unsigned int FrameBuffer::frameBufferId() const
{
	return m_frameBuffer;
}