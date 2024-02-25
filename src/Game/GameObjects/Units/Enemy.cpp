#include "Enemy.h"
#include "../Bullet.h"
#include "../../../Physics/PhysicsEngine.h"
#include "../../AIComponent.h"

Enemy::Enemy(
	const EOrientation eOrientation,
	const double maxVelocity,
	const glm::vec2& position,
	const glm::vec2& size, 
	const float layer) :
	IUnit("enemy", eOrientation, maxVelocity, position, size, layer)
{
	m_pAIComponent = std::make_unique<AIComponent>(this);	
}

void Enemy::update(const double delta)
{
	IUnit::update(delta);

	if (!m_isSpawning)	
		if (m_pAIComponent)		
			m_pAIComponent->update(delta);
}