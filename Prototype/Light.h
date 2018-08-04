#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm\common.hpp>
#include <glm\matrix.hpp>

class Light
{
protected:
	void calcSpaceMatrix();

	glm::vec3 m_position;
	glm::vec3 m_lookAtPos;
	glm::vec3 m_up;
	glm::mat4 m_spaceMatrix;

public:
	 Light(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up);
	 Light();
	~Light();

	void initialise(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up);

	void position (const glm::vec3& delta);
	const glm::vec3& position() const;

	const glm::mat4& spaceMatrix() const;
};

#endif
