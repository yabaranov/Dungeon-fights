#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include "../IGameObject.h"
#include "../../../Renderer/SpriteAnimator.h"
#include"../../../System/Timer.h"

#include <string>
#include <vector>

namespace RenderEngine
{
	class Sprite;
}

class Bullet;
class Blood;

class IUnit : public IGameObject
{
public:

	enum class EUnitState : uint8_t
	{
		Alive, Dead
	};

	enum class EOrientation : uint8_t
	{
		Top, Bottom, Left, Right
	};

	IUnit(const IGameObject::EObjectType unitType, const std::string& sprite, const EOrientation eOrientation, const double maxVelocity,
		const glm::vec2& position, const glm::vec2& size, const float layer);

	virtual void render() const override;
	virtual void setVelocity(const double velocity) override;
	virtual void update(const double delta) override;

	void setOrientation(const EOrientation eOrientation);
	EOrientation getOrientation() const { return m_eOrientation; }
	double getMaxVelocity() const { return m_maxVelocity; }
	void fire();
	EUnitState getUnitState() { return m_eUnitState; }
	unsigned int getDamage() { return m_damage; }
	bool hasHit() { return m_hit; }
	void setHit(bool hit) { m_hit = hit; }

protected:

	void bulletReaction(const IGameObject& object);
	bool m_hit;
	EUnitState m_eUnitState;
	int m_health;
	unsigned int m_damage;
	EOrientation m_eOrientation;
	std::shared_ptr<Bullet> m_pCurrentBullet;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite;
	std::shared_ptr<RenderEngine::Sprite> m_pSpriteShot;
	
	std::shared_ptr<RenderEngine::Sprite> m_pRespawnSprite;
	RenderEngine::SpriteAnimator m_respawnSpriteAnimator;

	std::shared_ptr<RenderEngine::Sprite> m_pDeathSprite;
	glm::vec2 m_deathPosition;

	Timer m_respawnTimer;
	Timer m_shotTimer;
	Timer m_deathTimer;
	
	double m_maxVelocity;
	bool m_isSpawning;
	glm::vec2 m_respawnPosition;

	glm::vec2 colliderOffset;

	std::vector<std::shared_ptr<Blood>> m_blood;
};