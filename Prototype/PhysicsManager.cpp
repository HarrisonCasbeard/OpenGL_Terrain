#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
	m_pBroadphase = new btDbvtBroadphase();
	m_pConfig = new btDefaultCollisionConfiguration();
	m_pCollisionDispatcher = new btCollisionDispatcher(m_pConfig);
	m_pSolver = new btSequentialImpulseConstraintSolver();
	m_pWorld = new btDiscreteDynamicsWorld(m_pCollisionDispatcher, m_pBroadphase, m_pSolver, m_pConfig);
	m_pWorld->setGravity({ 0.0f, -10.0f, 0.0f });
}

PhysicsManager::~PhysicsManager()
{
	delete m_pWorld;				m_pWorld = nullptr;
	delete m_pSolver;				m_pSolver = nullptr;
	delete m_pCollisionDispatcher;	m_pCollisionDispatcher = nullptr;
	delete m_pConfig;				m_pConfig = nullptr;
	delete m_pBroadphase;			m_pBroadphase = nullptr;
}
//-------------------------------------------------------------------------------------------------
bool PhysicsManager::update(double deltaTime)
{
	//This is messing up everything! I think it's taking too long or something
	//m_pWorld->performDiscreteCollisionDetection();
	
	//Equation for 2nd Parameter : http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Stepping_The_World
	m_pWorld->stepSimulation((btScalar)deltaTime, 7);

	//return collisionCheck(shipRb);

	return false;
}
//-------------------------------------------------------------------------------------------------
bool PhysicsManager::collisionCheck(const btCollisionObject* shipRb)
{
	if (!shipRb)
		return false;

	int numManifolds = m_pWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = m_pWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		if (shipRb == obA || shipRb == obB)
		{
			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
btDiscreteDynamicsWorld* const PhysicsManager::world() const
{
	return m_pWorld;
}