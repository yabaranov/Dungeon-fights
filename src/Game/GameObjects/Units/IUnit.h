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

	enum class EOrientation : uint8_t
	{
		Top, Bottom, Left, Right
	};

	IUnit(std::shared_ptr<RenderEngine::Sprite> sprite, const EOrientation eOrientation, const double maxVelocity, 
		const glm::vec2& position, const glm::vec2& size, const float layer);

	virtual void render() const override;
	virtual void setVelocity(const double velocity) override;

	void setOrientation(const EOrientation eOrientation);
	double getMaxVelocity() const { return m_maxVelocity; }
	void fire();

protected:
	EOrientation m_eOrientation;
	std::shared_ptr<Bullet> m_pCurrentBullet;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite;

	std::shared_ptr<RenderEngine::Sprite> m_pSprite_respawn;
	RenderEngine::SpriteAnimator m_spriteAnimator_respawn;

	Timer m_respawnTimer;

	double m_maxVelocity;
	bool m_isSpawning;
};