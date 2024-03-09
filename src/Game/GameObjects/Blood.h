#pragma once

#include "IGameObject.h"
#include"../../Renderer/SpriteAnimator.h"
#include"../../System/Timer.h"

#include <memory>

namespace RenderEngine
{
	class Sprite;
}

class BloodUnit : public IGameObject
{
public:

	BloodUnit(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	
	virtual void render() const override;
	virtual void update(const double delta) override;
	bool isActive() const { return m_isActive; }

private:
	bool m_isActive;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite;
	RenderEngine::SpriteAnimator m_pSpriteAnimator;
	Timer m_timer;
};

class Blood
{
public:
	Blood() {}
	void addBloodUnit(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	{
		m_blood.emplace_back(std::make_shared<BloodUnit>(position, size, rotation, layer - 0.01f));
	}
	void render() const;
	void update(const double delta);

private:
	std::vector<std::shared_ptr<BloodUnit>> m_blood;
};