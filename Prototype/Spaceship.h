#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include "GameObject3D.h"

class Spaceship : public GameObject3D
{
private:
	void boundaryCheck();
	
	float m_speed;
	glm::vec2 m_minBoundary;
	glm::vec2 m_maxBoundary;

public:
	 Spaceship(const float speed, Model& model, const glm::vec3& pos, Shader& shader, const glm::vec3& rot = { 0.0f, 0.0f, 0.0f }, const glm::vec3& scale = { 1.0f, 1.0f, 1.0f });
	~Spaceship();

	void update();

	void		 speed	(const float speed);
	const float& speed	() const;
};

#endif
