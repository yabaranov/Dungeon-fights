#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include "IGameState.h"

namespace RenderEngine
{
	class Sprite;
}

class Game;

class GameOver : public IGameState
{
public:
	GameOver(Game* pGame);
	virtual void render() const override;
	virtual void processInput(const std::array<bool, 349>& keys, const std::array<bool, 349>&) override;

	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;
private:
	Game* m_pGame;
	std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2> m_Sprite;
};