#pragma once

#include <array>
#include <memory>
#include <glm/vec2.hpp>

class IGameState;

namespace RenderEngine
{
	class ShaderProgram;
}

class Game
{
	
public:
	Game(const glm::uvec2& windowSize);
	~Game();
	void render();
	void update(const double delta);
	void setKey(const int key, const int action);
	bool init();
	size_t getCurrentWidth() const;
	size_t getCurrentHeight() const;

	void level(const size_t level);
	void gameOver();
	void win();
	void startScreen();
	void setWindowSize(const glm::uvec2& windowSize);
	void updateViewport();

private:
	std::array<bool, 349> m_keys;

	glm::uvec2 m_windowSize;

	std::shared_ptr<IGameState> m_pCurrentGameState;
	std::shared_ptr<RenderEngine::ShaderProgram> m_pSpriteShaderProgram;
};