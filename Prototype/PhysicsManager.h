#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "btBulletDynamicsCommon.h"

class PhysicsManager
{
private:
	bool collisionCheck	(const btCollisionObject* shipRb);

	btBroadphaseInterface*				 m_pBroadphase;
	btCollisionConfiguration*			 m_pConfig;
	btCollisionDispatcher*				 m_pCollisionDispatcher;
	btSequentialImpulseConstraintSolver* m_pSolver;
	btDiscreteDynamicsWorld*			 m_pWorld;

public:
	 PhysicsManager();
	~PhysicsManager();

	bool update			(double deltaTime);

	btDiscreteDynamicsWorld* const world() const;
};

#endif
