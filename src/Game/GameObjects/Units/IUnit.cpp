#include "IUnit.h"

#include "../../../Renderer/Sprite.h"
#include "../../../Resources/ResourceManager.h"
#include "../Bullet.h"
#include "../../../Physics/PhysicsEngine.h"

IUnit::IUnit(std::shared_ptr<RenderEngine::Sprite> sprite, const EOrientation eOrientation, const double maxVelocity,
	const glm::vec2& position, const glm::vec2& size, const float layer) :
	IGameObject(IGameObject::EObjectType::Unit, position, size, 0.f, layer), 
	m_pCurrentBullet(std::make_shared<Bullet>(0.1, m_position + m_size / 4.f, m_size / 2.f, m_size, layer)),
	m_pSprite(std::move(sprite)),
	m_pSprite_respawn(ResourceManager::getSprite("respawn")),
	m_spriteAnimator_respawn(m_pSprite_respawn),
	m_maxVelocity(maxVelocity),
	m_isSpawning(true)
{
	setOrientation(eOrientation);
}

void IUnit::render() const
{
	if (m_isSpawning)
	{
		m_pSprite_respawn->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_respawn.getCurrentFrame());
	}
	else
	{
		m_pSprite->render(m_position, m_size, m_rotation, m_layer);
	}

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

void IUnit::fire()
{
	if (!m_isSpawning && !m_pCurrentBullet->isActive())
	{
		m_pCurrentBullet->fire(m_position + m_size / 4.f + m_size * m_direction / 4.f, m_direction);
	}
}