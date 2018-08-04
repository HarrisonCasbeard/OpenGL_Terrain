#ifndef _GAME_OBJECT_3D_H_
#define _GAME_OBJECT_3D_H_

#include "Drawable.h"
#include "Model.h"
#include "btBulletDynamicsCommon.h"

class Shader;

class GameObject3D : public Drawable
{
protected:
	Model		m_model;
	glm::vec3	m_position;
	glm::vec3	m_rotation;
	glm::vec3	m_scale;

	btCollisionShape*	m_pCollisionShape;
	btMotionState*		m_pMotionState;
	btRigidBody*		m_pRigidBody;

public:
	GameObject3D(Model& model, const glm::vec3& pos, Shader& shader, const glm::vec3& rot = { 0.0f, 0.0f, 0.0f }, const glm::vec3& scale = { 1.0f, 1.0f, 1.0f });
	virtual ~GameObject3D();

	void render() override;

	void			 position(const glm::vec3& newPos);
	const glm::vec3& position() const;

	void			 rotation(const glm::vec3& newRot);
	const glm::vec3& rotation() const;

	void			 scale(const glm::vec3& newScale);
	const glm::vec3& scale() const;

	const glm::vec3  dimensions() const;

	void			 model(Model& model);
	const Model&	 model() const;

	void				rigidbody(btRigidBody* rigidbody, btDynamicsWorld* const world);
	void				rigidbody(btCollisionShape* shape, const int mass, const btVector3& inertia, btDynamicsWorld* const world);
	btRigidBody* const	rigidbody() const;

	void update();
};

#endif
