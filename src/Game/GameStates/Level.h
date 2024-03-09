#pragma once

#include <vector>
#include <string>
#include <memory>
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
	class Settings
	{
	public:
		friend class Level;
		Settings(std::vector<std::string> _levelDescription, unsigned int _enemyLives, unsigned int _playerLives,
		unsigned int _enemyHealth, unsigned int _playerHealth, unsigned int _enemyDamage, unsigned int _playerDamage) :
			levelDescription(std::move(_levelDescription)), enemyLives(_enemyLives), playerLives(_playerLives), 
			enemyHealth(_enemyHealth), playerHealth(_playerHealth), enemyDamage(_enemyDamage), playerDamage(_playerDamage)
		{
		}

	private:
		std::vector<std::string> levelDescription;
		unsigned int enemyLives;
		unsigned int playerLives;
		unsigned int enemyHealth;
		unsigned int playerHealth;
		unsigned int enemyDamage;
		unsigned int playerDamage;
	};

	static constexpr unsigned int BLOCK_SIZE = 16;
	Level(Game* pGame, const Settings& levelSettings);
	virtual void render() const override;
	virtual void update(const double delta) override;
	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;

	std::vector<std::shared_ptr<IGameObject>> getObjectsInArea(const glm::vec2& bottomLeft, const glm::vec2& topRight);

	virtual void processInput(const std::array<bool, 349>& keys, const std::array<bool, 349>& keysPressed) override;

	void initLevel();

private:
	Game* m_pGame;

	Settings m_levelSettings;

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