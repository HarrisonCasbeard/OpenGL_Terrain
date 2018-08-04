#include "GameObject3D.h"
#include "Shader.h"

GameObject3D::GameObject3D(Model& model, const glm::vec3& pos, Shader& shader, const glm::vec3& rot, const glm::vec3& scale)
{
	m_model		= model;
	m_position	= pos;
	m_rotation	= rot;
	m_scale		= scale;
	m_is3D		= false;
	m_pShader	= &shader;

	m_pCollisionShape = nullptr;
	m_pRigidBody = nullptr;
	m_pMotionState = nullptr;
}

GameObject3D::~GameObject3D()
{
	if (m_pRigidBody)
	{
		delete m_pRigidBody;
		m_pRigidBody = nullptr;
	}

	if (m_pMotionState)
	{
		delete m_pMotionState;
		m_pMotionState = nullptr;
	}

	if (m_pCollisionShape)
	{
		delete m_pCollisionShape;
		m_pCollisionShape = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------
void GameObject3D::update()
{
	if (m_pRigidBody)
	{
		btTransform trans;

		//m_pMotionState->getWorldTransform(trans);
		trans = m_pRigidBody->getWorldTransform();
		m_position = glm::vec3(	trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	}
}
//-------------------------------------------------------------------------------------------------
void GameObject3D::render()
{
	if (!m_pShader)
	{
		printf("Object3D has no shader! Skipping rendering... \n");
		return;
	}

	glm::mat4 modelMatrix = glm::mat4();
	glm::vec3 halfExtents = (m_model.dimensions() * m_scale) * 0.5f;
	modelMatrix[3] = glm::vec4(m_position, 1.0f);

	modelMatrix[0][0] = m_scale.x;
	modelMatrix[1][1] = m_scale.y;
	modelMatrix[2][2] = m_scale.z;

	m_pShader->activate();
	m_pShader->setMatrixf("model", &modelMatrix[0][0]);

	m_model.draw(*m_pShader);
}
//-------------------------------------------------------------------------------------------------
void GameObject3D::rigidbody(btRigidBody* rigidbody, btDynamicsWorld* const world)
{
	m_pRigidBody = rigidbody;

	if(world)
		world->addRigidBody(m_pRigidBody);
}
//-------------------------------------------------------------------------------------------------
void GameObject3D::rigidbody(btCollisionShape* shape, const int mass, const btVector3& inertia, btDynamicsWorld* const world)
{
	m_pCollisionShape = shape;
	btVector3 fallInertia = inertia;
	shape->calculateLocalInertia((btScalar)mass, fallInertia);

	m_pMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(	m_position.x, 
																								m_position.y,
																								m_position.z)));

	btTransform trans;
	m_pMotionState->getWorldTransform(trans);
	m_pRigidBody = new btRigidBody((btScalar)mass, m_pMotionState, m_pCollisionShape, fallInertia);

	if (world)
		world->addRigidBody(m_pRigidBody);
}
//-------------------------------------------------------------------------------------------------
btRigidBody* const GameObject3D::rigidbody() const
{
	return m_pRigidBody;
}
//-------------------------------------------------------------------------------------------------
void GameObject3D::model(Model& model)
{
	m_model = model;
}
//-------------------------------------------------------------------------------------------------
const Model& GameObject3D::model() const
{
	return m_model;
}
//-------------------------------------------------------------------------------------------------
void GameObject3D::position(const glm::vec3& newPos)
{
	m_position = newPos;
}
//-------------------------------------------------------------------------------------------------
const glm::vec3& GameObject3D::position() const
{
	return m_position;
}
//-------------------------------------------------------------------------------------------------
void GameObject3D::rotation(const glm::vec3& newRot)
{
	m_rotation = newRot;
}
//-------------------------------------------------------------------------------------------------
const glm::vec3& GameObject3D::rotation() const
{
	return m_rotation;
}
//-------------------------------------------------------------------------------------------------
void GameObject3D::scale(const glm::vec3& newScale)
{
	m_scale = newScale;
}
//-------------------------------------------------------------------------------------------------
const glm::vec3& GameObject3D::scale() const
{
	return m_scale;
}
//-------------------------------------------------------------------------------------------------
const glm::vec3 GameObject3D::dimensions() const
{
	return m_model.dimensions() * m_scale;
}