#include "Enemy.h"
#include "../../AIComponent.h"

Enemy::Enemy(const int health, const unsigned int damage, 
	const Player* pPlayer,
	const EOrientation eOrientation,
	const double maxVelocity,
	const glm::vec2& position,
	const glm::vec2& size, 
	const float layer) : 
	IUnit(IGameObject::EObjectType::Enemy, health, damage, eOrientation, maxVelocity, position, size, layer)
{
	m_pAIComponent = std::make_unique<AIComponent>(this, pPlayer);

	auto onCollisionCallback = [&](const IGameObject& object, const Physics::ECollisionDirection)
		{
			IUnit::bulletReaction(object);
			m_pAIComponent->setRandomOrientation();
		};

	m_colliders.emplace_back(colliderOffset, m_size - colliderOffset, onCollisionCallback);

}

void Enemy::update(const double delta)
{
	IUnit::update(delta);
	m_pAIComponent->update(delta);
}