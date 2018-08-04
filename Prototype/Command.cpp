#include <glfw3.h>

#include "Command.h"
#include "GameObject3D.h"
#include "Camera.h"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
ApplyRBForceCommand::ApplyRBForceCommand(const glm::vec3& force)
{
	m_force = force;
}


bool ApplyRBForceCommand::execute(GameObject3D& actor, const double deltaTime)
{
	if (&actor == nullptr)
		return true;

	if (!actor.rigidbody())
		return true;

	btScalar roundedTime = (btScalar)deltaTime;

	actor.rigidbody()->activate(true);
	actor.rigidbody()->applyCentralImpulse({ m_force.x * roundedTime, m_force.y * roundedTime, m_force.z * roundedTime });

	return true;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
bool GameCloseCommand::execute(GameObject3D& actor, const double deltaTime)
{
	return false;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------