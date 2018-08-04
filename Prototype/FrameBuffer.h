#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

class FrameBuffer
{
private:
	void createFrameBuffers(const int screenW, const int screenH);
	void createQuadBuffers();

	unsigned int m_frameBuffer;
	unsigned int m_textureBuffer;
	unsigned int m_renderBuffer;

	unsigned int m_quadVAO;
	unsigned int m_quadVBO;

	bool m_isEnabled;

public:
	 FrameBuffer(const int screenW, const int screenH);
	~FrameBuffer();

	void draw();

	void		enabled(const bool isEnabled);
	const bool	enabled() const;

	const unsigned int frameBufferId() const;
};

#endif
