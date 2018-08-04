#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "GameObject3D.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

//This class looks like shit - please fix later . . .
//-------------------------------------------------------------------------------------------------
void Camera::initialise(const glm::vec3& position, const glm::vec3& up, const float pitch, const float yaw, const float roll)
{
	m_position	= position;
	m_up		= up;

	m_speed = 55.5f; //per second;
	m_offset = 100.0f;

	m_pitch = pitch;
	m_yaw	= yaw;
	m_roll	= roll;

	m_pitchBoundary = 89.0f;

	updateFront();
}
//-------------------------------------------------------------------------------------------------
void Camera::update(const GameObject3D& object)
{
	m_position.z = object.position().z + m_offset;
}
//-------------------------------------------------------------------------------------------------
void Camera::updateFront()
{
	m_front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	m_front = glm::normalize(m_front);
}
//-------------------------------------------------------------------------------------------------
void Camera::movePosition(const glm::vec3& delta)
{
	m_position += delta;
}
//-------------------------------------------------------------------------------------------------
void Camera::addYawPitchRoll(const float pitch, const float yaw, const float roll)
{
	m_pitch += pitch;
	m_yaw += yaw;
	m_roll += roll;
}
//-------------------------------------------------------------------------------------------------
void Camera::addYaw(const float delta)
{
	m_yaw += delta;
}
//-------------------------------------------------------------------------------------------------
void Camera::addPitch(const float delta)
{
	m_pitch += delta;

	if (m_pitch > m_pitchBoundary)
		m_pitch = m_pitchBoundary;

	else if (m_pitch < -(m_pitchBoundary))
		m_pitch = -(m_pitchBoundary);
}
//-------------------------------------------------------------------------------------------------
void Camera::addRoll(const float delta)
{
	m_roll += delta;
}
//-------------------------------------------------------------------------------------------------
void Camera::setPosition(const glm::vec3& newPos)
{
	m_position = newPos;
}
//-------------------------------------------------------------------------------------------------
void Camera::setUpVector(const glm::vec3& newUp)
{
	m_up = newUp;
}
//-------------------------------------------------------------------------------------------------
void Camera::setFront(const glm::vec3& newFront)
{
	m_front = newFront;
}
//-------------------------------------------------------------------------------------------------
void Camera::setYawPitchRoll(const float yaw, const float pitch, const float roll)
{
	m_yaw = yaw;
	m_pitch = pitch;
	m_roll = roll;
}
//-------------------------------------------------------------------------------------------------
void Camera::setSpeed(const float speed)
{
	m_speed = speed;
}
//-------------------------------------------------------------------------------------------------
void Camera::setYaw(const float yaw)
{
	m_yaw = yaw;
}
//-------------------------------------------------------------------------------------------------
void Camera::setPitch(const float pitch)
{
	m_pitch = pitch;

	if (m_pitch > m_pitchBoundary)
		m_pitch = m_pitchBoundary;

	else if (m_pitch < -(m_pitchBoundary))
		m_pitch = -(m_pitchBoundary);

}
//-------------------------------------------------------------------------------------------------
void Camera::setRoll(const float roll)
{
	m_roll = roll;
}
//-------------------------------------------------------------------------------------------------
const glm::vec3& Camera::getPosition() const
{
	return m_position;
}
//-------------------------------------------------------------------------------------------------
const glm::vec3& Camera::getUp() const
{
	return m_up;
}
//-------------------------------------------------------------------------------------------------
const glm::vec3& Camera::getFront() const
{
	return m_front;
}
//-------------------------------------------------------------------------------------------------
const glm::mat4& Camera::getViewMatrix()
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
	return m_viewMatrix;
}
//-------------------------------------------------------------------------------------------------
const float Camera::getSpeed() const
{
	return m_speed;
}
//-------------------------------------------------------------------------------------------------
const float Camera::getYaw() const
{
	return m_yaw;
}
//-------------------------------------------------------------------------------------------------
const float Camera::getPitch() const
{
	return m_pitch;
}
//-------------------------------------------------------------------------------------------------
const float Camera::getRoll() const
{
	return m_roll;
}
//-------------------------------------------------------------------------------------------------