#ifndef _GAME_OBJECT_2D_H_
#define _GAME_OBJECT_2D_H_

#include "Drawable.h"
#include <string>

class Shader;
class GameObject2D : public Drawable
{
protected:
	//----- Init Functions -----
	void initBuffers();
	void initTexture(const std::string& filepath);
	void calcMatrix ();

	//----- Object Needs -----
	glm::vec2	m_position;
	glm::vec3	m_colour;
	glm::vec2	m_dimensions;
	glm::vec2	m_scale;
	float		m_rotation;
	glm::mat4	m_modelMatrix;

	//----- Rendering Variables -----
	unsigned int m_VBO;
	unsigned int m_VAO;
	unsigned int m_texID;

public:
	 GameObject2D(const std::string& filepath, const glm::vec2& pos, Shader& shader, const float rotInDeg = 0.0f, const glm::vec2& scale = glm::vec2( 1.0f, 1.0f ), const glm::vec3& colour = glm::vec3( 1.0f, 1.0f, 1.0f ));
	 GameObject2D() {}
	~GameObject2D();

	void initialise		(const std::string& filepath, const glm::vec2& pos, Shader& shader, const float rotInDeg = 0.0f, const glm::vec2& scale = glm::vec2( 1.0f, 1.0f ), const glm::vec3& colour = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual void render () override;

	//----- Functional Methods -----
	virtual void			 position(const glm::vec2& newPos);
	virtual const glm::vec2& position() const;

	virtual void			 rotation(const float rotInDeg);
	virtual const float		 rotation() const;

	virtual void			 scale(const glm::vec2& newScale);
	virtual const glm::vec2& scale() const;

	virtual void			 colour(const glm::vec3& newCol);
	virtual const glm::vec3& colour() const;

	virtual void incPosition(const glm::vec2& delta);
};

#endif
