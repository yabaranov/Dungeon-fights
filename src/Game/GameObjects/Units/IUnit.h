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

	enum class EUnitType : uint8_t
	{
		Player,
		Enemy
	};

	enum class EUnitHealth : uint8_t
	{
		Player = 20,
		Enemy = 10
	};

	enum class EUnitDamage : uint8_t
	{
		Player = 5,
		Enemy = 5
	};

	IUnit(const EUnitType unitType, const std::string& sprite, const EOrientation eOrientation, const double maxVelocity,
		const glm::vec2& position, const glm::vec2& size, const float layer);

	virtual void render() const override;
	virtual void setVelocity(const double velocity) override;
	virtual void update(const double delta) override;

	void setOrientation(const EOrientation eOrientation);
	double getMaxVelocity() const { return m_maxVelocity; }
	void fire();
	EUnitState getUnitState() { return m_eUnitState; }

protected:

	EUnitState m_eUnitState;
	int m_health;
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

	EUnitType m_unitType;
};