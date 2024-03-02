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
		Player,
		Enemy,
		Trees,
		Water,
		Blood
	};

	IGameObject(const EObjectType objectType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);

	virtual void render() const = 0;
	virtual void update(const double delta) {};
	virtual ~IGameObject();
	const glm::vec2& getCurrentPosition() const { return m_position; }
	const glm::vec2& getTargetPosition() const { return m_targetPosition; }
	const glm::vec2& getCurrentDirection() const { return m_direction; }

	void setCurrentPosition(const glm::vec2& position) { m_position = position; }
	void setTargetPosition(const glm::vec2& targetPosition) { m_targetPosition = targetPosition; }
	void setCurrentDirection(const glm::vec2& direction) { m_direction = direction; }
	virtual double getCurrentVelocity() const { return m_velocity; }
	virtual void setVelocity(const double velocity);

	const glm::vec2& getSize() const { return m_size; }
	const std::vector<Physics::Collider>& getColliders() const { return m_colliders; }
	EObjectType getObjectType() const { return m_objectType; }
	virtual bool collides(const EObjectType objectType) { return true; }
	virtual void onCollision() {}

protected:
	glm::vec2 m_position;
	glm::vec2 m_targetPosition;
	double m_velocity;
	glm::vec2 m_size;
	float m_rotation;
	float m_layer;
	EObjectType m_objectType;
	glm::vec2 m_direction;	
	std::vector<Physics::Collider> m_colliders;
};