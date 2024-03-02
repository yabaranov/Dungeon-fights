#pragma once 

#include <glm/vec2.hpp>

#include "../../Physics/PhysicsEngine.h"

class IGameObject
{
public:

	enum class EObjectType : uint8_t
	{
		BetonWall,
		Border,
		BrickWall,
		Bullet,
		Unit,
		Trees,
		Water,
		Blood
	};

	IGameObject(const EObjectType objectType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	void setOwner(IGameObject* pOwner) { m_pOwner = pOwner; }
	IGameObject* getOwner() const { return m_pOwner; }

	virtual void render() const = 0;
	virtual void update(const double delta) {};
	virtual ~IGameObject();
	virtual glm::vec2& getCurrentPosition()  { return m_position; }
	virtual glm::vec2& getTargetPosition() { return m_targetPosition; }
	virtual glm::vec2& getCurrentDirection() { return m_direction; }
	virtual double getCurrentVelocity() const { return m_velocity; }
	virtual void setVelocity(const double velocity);

	const glm::vec2& getSize() const { return m_size; }
	const std::vector<Physics::Collider>& getColliders() const { return m_colliders; }
	EObjectType getObjectType() const { return m_objectType; }
	virtual bool collides(const EObjectType objectType) { return true; }
	virtual void onCollision() {}
	unsigned int getDamage() { return m_damage; }

protected:
	IGameObject* m_pOwner;
	glm::vec2 m_position;
	glm::vec2 m_targetPosition;
	glm::vec2 m_size;
	float m_rotation;
	float m_layer;
	EObjectType m_objectType;

	unsigned int m_damage;
	glm::vec2 m_direction;
	double m_velocity;
	std::vector<Physics::Collider> m_colliders;
};