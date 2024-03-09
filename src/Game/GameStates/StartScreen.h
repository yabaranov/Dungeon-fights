#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include "IGameState.h"

namespace RenderEngine
{
	class Sprite;
}

class Game;

class StartScreen : public IGameState
{
public:

	StartScreen(Game* pGame);
	virtual void render() const override;
	virtual void processInput(const std::array<bool, 349>& keys, const std::array<bool, 349>& keysPressed) override;

	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;
private:
	Game* m_pGame;
	int m_currentMenuSelection;

	std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2> m_titleSprite;
	std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2> m_menuSprite;
	std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2> m_playerSprite;
};