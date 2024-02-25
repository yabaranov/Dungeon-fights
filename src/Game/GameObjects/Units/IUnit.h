#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include "../IGameObject.h"
#include "../../../Renderer/SpriteAnimator.h"
#include"../../../System/Timer.h"

#include <string>

namespace RenderEngine
{
	class Sprite;
}

class Bullet;

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

	IUnit(const std::string& sprite, const EOrientation eOrientation, const double maxVelocity,
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

	Timer m_respawnTimer;
	Timer m_shotTimer;
	
	double m_maxVelocity;
	bool m_isSpawning;

	glm::vec2 colliderOffset;
	bool m_isBlood;
	std::shared_ptr<RenderEngine::Sprite> m_pSpriteBlood;
	Timer m_bloodTimer;
	glm::vec2 m_bloodPosition;
};