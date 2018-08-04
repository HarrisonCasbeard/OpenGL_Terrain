#ifndef _WATER_H_
#define _WATER_H_

#include "Drawable.h"
#include <glm\common.hpp>

class Shader;

class Water : public Drawable
{
private:
	void initialise(Shader& shader, const glm::vec3& pos, const glm::vec3& scale);
	void setupBuffers();

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_texID;

	glm::vec3 m_position;
	glm::vec3 m_scale;

public:
	Water(Shader& shader, const glm::vec3& pos = { 0.0f, 0.0f, 0.0f }, const glm::vec3& scale = { 1.0f, 1.0f, 1.0f});
	~Water();

	virtual void render() override;
};

#endif
