#pragma once

#include <memory>
#include <vector>
#include <string>
#include <glm/vec2.hpp>

#include "IGameState.h"
#include "../../Renderer/SpriteAnimator.h"

namespace RenderEngine
{
	class Sprite;
}

class Game;

class GameOver : public IGameState
{
public:
	static constexpr unsigned int BLOCK_SIZE = 8;
	static constexpr unsigned int SCREEN_WIDTH = 15 * BLOCK_SIZE * 2;
	static constexpr unsigned int SCREEN_HEIGHT = 14 * BLOCK_SIZE * 2;
	static constexpr unsigned int GAME_OVER_WIDTH = 140;
	static constexpr unsigned int GAME_OVER_HEIGHT = 70;


	GameOver(Game* pGame);
	virtual void render() const override;
	virtual void update(const double delta) override;
	virtual void processInput(const std::array<bool, 349>& keys) override;

	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;
private:
	Game* m_pGame;
	std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2> m_Sprite;
};