#include "IUnit.h"

#include "../../../Renderer/Sprite.h"
#include "../../../Resources/ResourceManager.h"
#include "../Bullet.h"
#include "../../../Physics/PhysicsEngine.h"

IUnit::IUnit(const EUnitType unitType, const std::string& sprite, const EOrientation eOrientation, const double maxVelocity,
	const glm::vec2& position, const glm::vec2& size, const float layer) : 
	m_respawnPosition(position),
	m_unitType(unitType),
	IGameObject(IGameObject::EObjectType::Unit, position, size, 0.f, layer),
	m_pCurrentBullet(std::make_shared<Bullet>(0.1, m_position + m_size / 4.f, m_size / 2.f, m_size / 2.f, layer + 0.01f)),
	m_pSprite(ResourceManager::getSprite(sprite)),
	m_pSpriteShot(ResourceManager::getSprite(sprite + "_shot")),
	m_pRespawnSprite(ResourceManager::getSprite("respawn")),
	m_pSpriteBlood(ResourceManager::getSprite("blood")),
	m_pDeathSprite(ResourceManager::getSprite(sprite + "_death")), 
	m_bloodPosition(position),
	m_respawnSpriteAnimator(m_pRespawnSprite),
	m_maxVelocity(maxVelocity),
	m_isSpawning(true),
	m_isBlood(false),
	colliderOffset(4.f), 
	m_eUnitState(EUnitState::Alive) 		
{
	switch (m_unitType)
	{
	case EUnitType::Player:
		m_health = static_cast<int>(EUnitHealth::Player);
		m_damage = static_cast<unsigned int>(EUnitDamage::Player);
		break;
	case EUnitType::Enemy:
		m_health = static_cast<int>(EUnitHealth::Enemy);
		m_damage = static_cast<unsigned int>(EUnitDamage::Enemy);
		break;
	}

	m_respawnTimer.setCallback([&]()
		{
			m_isSpawning = false;
		});

	m_respawnTimer.start(1500);

	auto onCollisionCallback = [&](const IGameObject& object, const Physics::ECollisionDirection)
		{
			if (object.getObjectType() == IGameObject::EObjectType::Bullet && !m_isSpawning && m_eUnitState == EUnitState::Alive)
			{
				m_bloodPosition = m_position;
				m_isBlood = true;
				m_bloodTimer.start(10000);
				m_health -= object.getOwner()->getDamage();
				if (m_health <= 0)
				{
					m_eUnitState = EUnitState::Dead;
					m_deathPosition = m_position;
					m_deathTimer.start(2000);
				}
					
			}
			
		};

	m_deathTimer.setCallback([&]()
		{
			m_eUnitState = EUnitState::Alive;

			switch (m_unitType)
			{
			case EUnitType::Player:
				m_health = static_cast<int>(EUnitHealth::Player);
				break;
			case EUnitType::Enemy:
				m_health = static_cast<int>(EUnitHealth::Enemy);
				break;
			}

			m_targetPosition = m_position = m_respawnPosition;
			m_isSpawning = true;			
			m_respawnTimer.start(1500);
		}
	);

	m_bloodTimer.setCallback([&]()
		{
			m_isBlood = false;
		}
	);

	setOrientation(eOrientation);

	m_colliders.emplace_back(colliderOffset, m_size - colliderOffset, onCollisionCallback);

	m_pCurrentBullet->setOwner(this);
	Physics::PhysicsEngine::addDynamicGameObject(m_pCurrentBullet);
}

void IUnit::render() const
{
	if (m_isSpawning)
	{
		m_pRespawnSprite->render(m_respawnPosition, m_size, m_rotation, m_layer, m_respawnSpriteAnimator.getCurrentFrame());
	}
	else if(m_eUnitState == EUnitState::Alive)
	{
		m_pSprite->render(m_position, m_size, m_rotation, m_layer);
	}
	else if(m_eUnitState == EUnitState::Dead)
	{
		m_pDeathSprite->render(m_deathPosition, m_size, m_rotation, m_layer);
	}

	if (m_isBlood)
		m_pSpriteBlood->render(m_bloodPosition, m_size, 0.f, m_layer - 0.01f);

	if (m_pCurrentBullet->isActive())
		m_pCurrentBullet->render();

}

void IUnit::setOrientation(const EOrientation eOrientation)
{
	m_eOrientation = eOrientation;

	switch (m_eOrientation)
	{
	case EOrientation::Top:
		m_direction.x = 0.f;
		m_direction.y = 1.f;
		break;
	case EOrientation::Bottom:
		m_direction.x = 0.f;
		m_direction.y = -1.f;
		break;
	case EOrientation::Left:
		m_direction.x = -1.f;
		m_direction.y = 0.f;
		break;
	case EOrientation::Right:
		m_direction.x = 1.f;
		m_direction.y = 0.f;
		break;
	}
}

void IUnit::setVelocity(const double velocity)
{
	if (!m_isSpawning)
	{
		m_velocity = velocity;
	}

}

void IUnit::update(const double delta)
{
	if (m_pCurrentBullet->isActive())
	{
		m_pCurrentBullet->update(delta);
		m_shotTimer.update(delta);
	}
		
	if (m_isSpawning)
	{
		m_respawnSpriteAnimator.update(delta);
		m_respawnTimer.update(delta);
	}
	else if(m_eUnitState == EUnitState::Alive)
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
	else if (m_eUnitState == EUnitState::Dead)
		m_deathTimer.update(delta);

	if (m_isBlood)
		m_bloodTimer.update(delta);

}

void IUnit::fire()
{
	if (!m_isSpawning && !m_pCurrentBullet->isActive())
	{
		std::swap(m_pSprite, m_pSpriteShot);

		m_shotTimer.setCallback([&]()
			{
				std::swap(m_pSprite, m_pSpriteShot);
			});

		m_shotTimer.start(150);

		m_pCurrentBullet->fire(m_position + m_size / 4.f + m_size * m_direction / 4.f, m_direction);
	}
}

