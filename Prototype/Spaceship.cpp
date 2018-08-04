#include "Spaceship.h"

Spaceship::Spaceship(const float speed, Model& model, const glm::vec3& pos, Shader& shader, const glm::vec3& rot, const glm::vec3& scale)
	: GameObject3D(model, pos, shader, rot, scale)
{
	m_speed = speed;
	m_minBoundary = { -24.0f, -35.0f };
	m_maxBoundary = {  44.0f,  35.0f };
}

Spaceship::~Spaceship()
{
}
//-------------------------------------------------------------------------------------------------
void Spaceship::update()
{
	auto curVel = m_pRigidBody->getLinearVelocity();
	m_pRigidBody->setLinearVelocity({ curVel.x(), curVel.y(), m_speed });
	
	boundaryCheck();

	GameObject3D::update();
}
//-------------------------------------------------------------------------------------------------
void Spaceship::boundaryCheck()
{
	btTransform curPos = m_pRigidBody->getWorldTransform();

	if (curPos.getOrigin().x() <= m_minBoundary.x)
	{
		curPos.getOrigin().setX(m_minBoundary.x + 0.01f);
		m_pRigidBody->setWorldTransform(curPos);
		m_pRigidBody->clearForces();
	}
	else if (curPos.getOrigin().x() >= m_maxBoundary.x)
	{
		curPos.getOrigin().setX(m_maxBoundary.x - 0.01f);
		m_pRigidBody->setWorldTransform(curPos);
		m_pRigidBody->clearForces();
	}

	if (curPos.getOrigin().y() <= m_minBoundary.y)
	{
		curPos.getOrigin().setY(m_minBoundary.y + 0.01f);
		m_pRigidBody->setWorldTransform(curPos);
		m_pRigidBody->clearForces();
	}
	else if (curPos.getOrigin().y() >= m_maxBoundary.y)
	{
		curPos.getOrigin().setY(m_maxBoundary.y - 0.01f);
		m_pRigidBody->setWorldTransform(curPos);
		m_pRigidBody->clearForces();
	}
}
//-------------------------------------------------------------------------------------------------
void Spaceship::speed(const float speed)
{
	m_speed = speed;
}
//-------------------------------------------------------------------------------------------------
const float& Spaceship::speed() const
{
	return m_speed;
}