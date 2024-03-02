#include "Player.h"
#include "../../../Renderer/Sprite.h"
#include "../../../Resources/ResourceManager.h"
#include "../Bullet.h"
#include "../../../Physics/PhysicsEngine.h"

Player::Player(const EOrientation eOrientation,
	const double maxVelocity,
	const glm::vec2& position,
	const glm::vec2& size,
	const float layer) : 
	IUnit(EUnitType::Player, "player", eOrientation, maxVelocity, position, size, layer)
{	
	auto onCollisionCallback = [&](const IGameObject& object, const Physics::ECollisionDirection)
		{
			IUnit::bulletReaction(object);
		};

	m_colliders.emplace_back(colliderOffset, m_size - colliderOffset, onCollisionCallback);
}