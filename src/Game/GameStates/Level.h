#pragma once

#include <vector>
#include <array>
#include <string>
#include <memory>
#include <set>
#include <glm/vec2.hpp>

#include "IGameState.h"
#include"../GameObjects/Units/IUnit.h"

class IGameObject;
class Enemy;
class Player;
class Game;

class Level : public IGameState
{
public:
	static constexpr unsigned int BLOCK_SIZE = 16;
	Level(Game* pGame, const std::vector<std::string>& levelDescription);
	virtual void render() const override;
	virtual void update(const double delta) override;
	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;

	std::vector<std::shared_ptr<IGameObject>> getObjectsInArea(const glm::vec2& bottomLeft, const glm::vec2& topRight);

	virtual void processInput(const std::array<bool, 349>& keys, const std::array<bool, 349>& keysPressed) override;

	void initLevel();

private:
	Game* m_pGame;

	size_t m_widthBlocks;
	size_t m_heightBlocks;

	unsigned int m_widthPixels = 0;
	unsigned int m_heightPixels = 0;

	glm::ivec2 m_playerRespawn;
	unsigned int m_playerLives;
	IUnit::EUnitState m_playerState;

	std::vector<glm::ivec2> m_enemyRespawns;
	std::vector<unsigned int> m_enemyLives;
	std::vector<Timer> m_enemyDeathTimers;
	std::vector<IUnit::EUnitState> m_enemyStates;

	std::vector<std::shared_ptr<IGameObject>> m_levelObjects;
	std::shared_ptr<Player> m_pPlayer;
	std::pair<bool, Timer> m_nextStateTimer;
	std::vector<std::shared_ptr<Enemy>> m_enemies;
};