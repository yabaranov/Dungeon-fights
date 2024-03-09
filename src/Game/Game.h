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
	void render() const;
	void update(const double delta);
	void setKey(const int key, const int action);
	void setKeyPressed(const int key, const int action);
	bool init();
	size_t getCurrentWidth() const;
	size_t getCurrentHeight() const;

	void level(const size_t level);
	void gameOver();
	void win();
	void startScreen();

	void setWindowSize(const glm::uvec2& windowSize);
	void updateViewport();
	size_t getCurrentLevel() const { return m_currentLevel; }
	size_t getMaxLevel() const { return m_maxLevel; }

private:
	std::array<bool, 349> m_keys;
	std::array<bool, 349> m_keysPressed;
	glm::uvec2 m_windowSize;

	std::shared_ptr<IGameState> m_pCurrentGameState;
	std::shared_ptr<RenderEngine::ShaderProgram> m_pSpriteShaderProgram;

	size_t m_currentLevel;
	size_t m_maxLevel;
};