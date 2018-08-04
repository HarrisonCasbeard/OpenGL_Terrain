#ifndef	_WATER_FRAME_BUFFER_H_
#define _WATER_FRAME_BUFFER_H_

#include <glew\glew.h>
#include <vector>

class WaterFrameBuffer
{
private:
	GLuint m_FBO;
	GLuint m_texID;
	GLuint m_renderBuffer;
	
	void initFrameBuffer();

public:
	 WaterFrameBuffer();
	~WaterFrameBuffer();

	void bind(const int width, const int height);
	void unbind(const int width, const int height);

	void addColourTextureAttachment(const int width, const int height);
	void addDepthTextureAttachment(const int width, const int height);
	void addDepthBufferAttachment(const int width, const int height);
};

#endif
