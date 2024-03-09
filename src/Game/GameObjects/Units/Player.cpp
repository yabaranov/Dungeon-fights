#include "Player.h"

Player::Player(const int health, const unsigned int damage, 
	const EOrientation eOrientation,
	const double maxVelocity,
	const glm::vec2& position,
	const glm::vec2& size,
	const float layer) : 
	IUnit(IGameObject::EObjectType::Player, health, damage, eOrientation, maxVelocity, position, size, layer), m_inTrees(false)
{	
	auto onCollisionCallback = [&](const IGameObject& object, const Physics::ECollisionDirection)
		{
			IUnit::bulletReaction(object);
		};

	m_colliders.emplace_back(colliderOffset, m_size - colliderOffset, onCollisionCallback);
}