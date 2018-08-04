#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/vec3.hpp>
#include <glm\mat4x4.hpp>

class GameObject3D;

class Camera
{
protected:
	glm::vec3 m_position;
	glm::vec3 m_up;
	glm::vec3 m_front;
	glm::mat4 m_viewMatrix;

	float m_speed;
	float m_offset;

	float m_yaw;
	float m_pitch; 
	float m_pitchBoundary;
	float m_roll;

public:
	 Camera();
	~Camera();

	void initialise(const glm::vec3& position, const glm::vec3& up, const float pitch, const float yaw, const float roll);
	void updateFront();
	void update(const GameObject3D& object);

	//----- Modifiers ------
	void movePosition		(const glm::vec3& delta);
	void addYawPitchRoll	(const float pitch, const float yaw, const float roll);

	void addYaw		(const float delta);
	void addPitch	(const float delta);
	void addRoll	(const float delta);

	//----- Setters ------
	void setPosition	(const glm::vec3& newPos);
	void setUpVector	(const glm::vec3& newUp);
	void setFront		(const glm::vec3& newFront);
	void setYawPitchRoll(const float yaw, const float pitch, const float roll);

	void setSpeed (const float speed);
	void setYaw	  (const float yaw);
	void setPitch (const float pitch);
	void setRoll  (const float roll);

	//----- Getters ------
	const glm::vec3& getPosition	() const;
	const glm::vec3& getUp			() const;
	const glm::vec3& getFront		() const;
	const glm::mat4& getViewMatrix	();

	const float getSpeed() const;
	const float getYaw	() const;
	const float getPitch() const;
	const float getRoll	() const;
};

#endif
