#pragma once

#include <vector>
#include <string>
#include <memory>
#include <set>
#include <array>
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

	virtual void processInput(const std::array<bool, 349>& keys) override;

	void initLevel();

private:
	Game* m_pGame;

	static constexpr unsigned int NUMBER_ENEMIES = 3;
	static constexpr unsigned int NUMBER_ENEMY_LIVES = 5;
	static constexpr unsigned int NUMBER_PLAYER_LIVES = 5;
	size_t m_widthBlocks;
	size_t m_heightBlocks;

	unsigned int m_widthPixels = 0;
	unsigned int m_heightPixels = 0;

	glm::ivec2 m_playerRespawn;
	unsigned int m_playerLives;
	IUnit::EUnitState m_playerState;

	std::array<glm::ivec2, NUMBER_ENEMIES> m_enemyRespawns;
	std::array<unsigned int, NUMBER_ENEMIES> m_enemyLives;
	std::array<IUnit::EUnitState, NUMBER_ENEMIES> m_enemyStates;

	std::vector<std::shared_ptr<IGameObject>> m_levelObjects;
	std::shared_ptr<Player> m_pPlayer;
	std::array<std::shared_ptr<Enemy>, NUMBER_ENEMIES> m_enemies;
};