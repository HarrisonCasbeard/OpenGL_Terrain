#ifndef	SHADOW_MAP_H_
#define SHADOW_MAP_H_

#include <glm\common.hpp>
#include <glew\glew.h>

class ShadowMap
{
private:
	GLuint m_FBO;
	GLuint m_texID;
	glm::ivec2 m_resolution;


public:
	 ShadowMap(const glm::ivec2& res = { 1024, 1024 });
	~ShadowMap();

	void initialise();
	void bind	();
	void unbind	(const glm::ivec2& screenDimensions);

	const GLuint textureID() const;
};

#endif