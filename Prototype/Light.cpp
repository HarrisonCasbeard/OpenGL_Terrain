#include "Light.h"
#include <glm\gtc\matrix_transform.hpp>

Light::Light()
{}

Light::Light(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up)
{
	initialise(pos, lookAt, up);
}

Light::~Light()
{
}

//-------------------------------------------------------------------------------------------------
void Light::initialise(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up)
{
	m_position = pos;
	m_lookAtPos = lookAt;
	m_up = up;

	calcSpaceMatrix();
}
//-------------------------------------------------------------------------------------------------
void Light::calcSpaceMatrix()
{
	float near = 0.01f;
	float far = 100.0f;

	glm::mat4 lightProj = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, near, far);
	glm::mat4 lightView = glm::lookAt(m_position, m_lookAtPos, m_up);
	m_spaceMatrix = lightProj * lightView;
}
//-------------------------------------------------------------------------------------------------
void Light::position(const glm::vec3& delta)
{
	m_position += delta;
	calcSpaceMatrix();
}
//-------------------------------------------------------------------------------------------------
const glm::vec3& Light::position() const
{
	return m_position;
}
//-------------------------------------------------------------------------------------------------
const glm::mat4& Light::spaceMatrix() const
{
	return m_spaceMatrix;
}