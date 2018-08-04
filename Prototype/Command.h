#ifndef	_COMMAND_H_
#define _COMMAND_H_

#include <glm\glm.hpp>

class GameObject3D;
class Gun;

//-------------------------------------------------------------------------------------------------
class Command
{
public:
	virtual ~Command() {}

	enum class eActors{ SHIP, GUN, NUM_OF_ACTORS };

	eActors m_actor = eActors::SHIP;
	virtual bool execute(GameObject3D& actor, const double deltaTime) = 0;
	virtual bool execute(Gun& gun) { return true; }
};
//-------------------------------------------------------------------------------------------------
class ApplyRBForceCommand : public Command
{
private:
	glm::vec3 m_force;

public:
	ApplyRBForceCommand(const glm::vec3& force);

	virtual bool execute(GameObject3D& actor, const double deltaTime) override;
};
//-------------------------------------------------------------------------------------------------
class GameCloseCommand : public Command
{
private:

public:
	GameCloseCommand() {}

	virtual bool execute(GameObject3D& actor, const double deltaTime) override;
};
//-------------------------------------------------------------------------------------------------
#endif

