#pragma once

#include "IGameObject.h"
#include"../../Renderer/SpriteAnimator.h"
#include"../../System/Timer.h"

#include <array>
#include <memory>

namespace RenderEngine
{
	class Sprite;
}

class Blood : public IGameObject
{
public:

	Blood(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	
	virtual void render() const override;
	virtual void update(const double delta) override;
	bool isActive() { return m_isActive; }

private:
	bool m_isActive;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite;
	RenderEngine::SpriteAnimator m_pSpriteAnimator;
	Timer m_timer;
};