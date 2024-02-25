#include "Enemy.h"
#include "../../../Renderer/Sprite.h"
#include "../../../Resources/ResourceManager.h"
#include "../Bullet.h"
#include "../../../Physics/PhysicsEngine.h"
#include "../../AIComponent.h"

Enemy::Enemy(
	const EOrientation eOrientation,
	const double maxVelocity,
	const glm::vec2& position,
	const glm::vec2& size, 
	const float layer) :
	IUnit(ResourceManager::getSprite("enemy"), eOrientation, maxVelocity, position, size, layer)
{
	m_respawnTimer.setCallback([&]()
		{
			m_isSpawning = false;
			if (m_pAIComponent)
				m_velocity = m_maxVelocity;
		});

	m_respawnTimer.start(1500);

	m_colliders.emplace_back(glm::vec2(0), m_size);

	m_pCurrentBullet->setOwner(this);
	Physics::PhysicsEngine::addDynamicGameObject(m_pCurrentBullet);

	m_pAIComponent = std::make_unique<AIComponent>(this);
	
}

void Enemy::update(const double delta)
{
	if (m_pCurrentBullet->isActive())
		m_pCurrentBullet->update(delta);

	if (m_isSpawning)
	{
		m_spriteAnimator_respawn.update(delta);
		m_respawnTimer.update(delta);
	}
	else
	{
		if (m_pAIComponent)
		{
			m_pAIComponent->update(delta);
		}

		if (m_velocity > 0)
		{

			switch (m_eOrientation)
			{
			case EOrientation::Top:

				m_rotation = 90.0f;
				break;
			case EOrientation::Bottom:
				m_rotation = -90.0f;
				break;
			case EOrientation::Left:
				m_rotation = 180.0f;
				break;
			case EOrientation::Right:
				m_rotation = 0;
				break;
			default:
				break;
			}
		}
	}
}