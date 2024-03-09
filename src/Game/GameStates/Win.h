#pragma once

#include <memory>

#include "IGameState.h"
#include <glm/vec2.hpp>

namespace RenderEngine
{
	class Sprite;
}

class Game;

class Win : public IGameState
{
public:
	Win(Game* pGame);
	virtual void render() const override;
	virtual void processInput(const std::array<bool, 349>& keys, const std::array<bool, 349>&) override;

	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;
private:
	Game* m_pGame;
	std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2> m_Sprite;
};