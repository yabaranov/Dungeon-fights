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

	void startNewLevel(const size_t level);
	void nextLevel();
	void setWindowSize(const glm::uvec2& windowSize);
	void updateViewport();

private:
	enum class EGameState
	{
		StartScreen,
		Level,
		GameOver
	};
	std::array<bool, 349> m_keys;

	EGameState m_eCurrentGameState;
	glm::uvec2 m_windowSize;

	std::shared_ptr<IGameState> m_pCurrentGameState;
	std::shared_ptr<RenderEngine::ShaderProgram> m_pSpriteShaderProgram;
	size_t m_currentLevel;
};