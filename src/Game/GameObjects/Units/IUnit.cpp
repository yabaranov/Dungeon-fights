#include "IUnit.h"

#include "../../../Renderer/Sprite.h"
#include "../../../Resources/ResourceManager.h"
#include "../Bullet.h"
#include "../Blood.h"
#include "../../../Physics/PhysicsEngine.h"

IUnit::IUnit(const IGameObject::EObjectType unitType, const std::string& sprite, const EOrientation eOrientation, const double maxVelocity,
	const glm::vec2& position, const glm::vec2& size, const float layer) : 
	IGameObject(unitType, position, size, 0.f, layer),
	m_hit(true),
	m_respawnPosition(position),	
	m_pCurrentBullet(std::make_shared<Bullet>(this, 0.1, m_position + m_size / 4.f, m_size / 2.f, m_size / 2.f, layer + 0.01f)),
	m_pSprite(ResourceManager::getSprite(sprite)),
	m_pSpriteShot(ResourceManager::getSprite(sprite + "_shot")),
	m_pRespawnSprite(ResourceManager::getSprite("respawn")),
	m_respawnSpriteAnimator(m_pRespawnSprite),
	m_pDeathSprite(ResourceManager::getSprite(sprite + "_death")), 
	m_maxVelocity(maxVelocity),
	m_isSpawning(true),
	colliderOffset(4.f), 
	m_eUnitState(EUnitState::Alive) 		
{
	switch (m_objectType)
	{
	case IGameObject::EObjectType::Player:
		m_health = ResourceManager::getPlayerHealth();
		m_damage = ResourceManager::getPlayerDamage();
		break;
	case IGameObject::EObjectType::Enemy:
		m_health = ResourceManager::getEnemyHealth();
		m_damage = ResourceManager::getEnemyDamage();
		break;
	}

	m_respawnTimer.setCallback([&]()
		{
			m_isSpawning = false;
			m_eUnitState = EUnitState::Alive;
			m_velocity = m_maxVelocity;
		});

	m_respawnTimer.start(1500);

	m_deathTimer.setCallback([&]()
		{
			switch (m_objectType)
			{
			case IGameObject::EObjectType::Player:
				m_health = ResourceManager::getPlayerHealth();
				break;
			case IGameObject::EObjectType::Enemy:
				m_health = ResourceManager::getEnemyHealth();
				break;
			}
			m_isSpawning = true;			
			m_respawnTimer.start(1500);
		}
	);

	setOrientation(eOrientation);

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
		m_pDeathSprite->render(m_deathPosition, m_size, m_rotation, m_layer - 0.01f);
	}

	for (auto& currentBlood : m_blood)	
		if (currentBlood->isActive())
			currentBlood->render();
	
	if (m_pCurrentBullet->isActive())
		m_pCurrentBullet->render();

}

void IUnit::setOrientation(const EOrientation eOrientation)
{
	if (m_eUnitState == IUnit::EUnitState::Alive)
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
}

void IUnit::setVelocity(const double velocity)
{
	if (m_eUnitState == IUnit::EUnitState::Alive)
	{
		if (!m_isSpawning)
		{
			m_velocity = velocity;
		}
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

	for (size_t i = 0; i < m_blood.size(); i++)
		if (m_blood[i]->isActive())
			m_blood[i]->update(delta);
		else
			m_blood.erase(m_blood.begin() + i);
}

void IUnit::fire()
{
	if (!m_isSpawning && !m_pCurrentBullet->isActive() && m_eUnitState == IUnit::EUnitState::Alive)
	{
		std::swap(m_pSprite, m_pSpriteShot);

		m_shotTimer.setCallback([&]()
			{
				std::swap(m_pSprite, m_pSpriteShot);
			});

		m_shotTimer.start(150);

		m_pCurrentBullet->fire(m_position + m_size / 4.f + m_size * m_direction / 2.f, m_direction);
	}
}

void IUnit::bulletReaction(const IGameObject& object)
{
	if (object.getObjectType() == IGameObject::EObjectType::Bullet && !m_isSpawning && m_eUnitState == EUnitState::Alive)
	{
		m_blood.emplace_back(std::make_shared<Blood>(m_position, m_size, m_rotation, m_layer - 0.01f));
		
		m_health -= dynamic_cast<const Bullet&>(object).getOwner()->getDamage();
		if (m_health <= 0)
		{
			m_eUnitState = EUnitState::Dead;
			m_deathPosition = m_position;
			m_targetPosition = m_position = m_respawnPosition;
			m_deathTimer.start(2000);
		}
	}
}

