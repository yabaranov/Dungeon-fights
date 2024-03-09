#include "Level.h"

#include "../GameObjects/BrickWall.h"
#include "../GameObjects/BetonWall.h"
#include "../GameObjects/Trees.h"
#include "../GameObjects/Water.h"
#include "../GameObjects/Border.h"
#include "../GameObjects/Units/Enemy.h"
#include "../GameObjects/Units/Player.h"

#include "../Game.h"

#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>

#include <iostream>
#include <algorithm>

std::shared_ptr<IGameObject> createGameObjectFromDescription(const char description, const glm::vec2& position, 
	const glm::vec2& size, const float rotation)
{
	switch (description)
	{		
	case '0':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Right, position, size, rotation, 0.f);
	case '1':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Bottom, position, size, rotation, 0.f);
	case '2':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Left, position, size, rotation, 0.f);
	case '3':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Top, position, size, rotation, 0.f);
	case '4':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::All, position, size, rotation, 0.f);
	case 'G':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::BottomLeft, position, size, rotation, 0.f);
	case 'H':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::BottomRight, position, size, rotation, 0.f);
	case 'I':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::TopLeft, position, size, rotation, 0.f);
	case 'J':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::TopRight, position, size, rotation, 0.f);
	case '5':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Right, position, size, rotation, 0.f);
	case '6':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Bottom, position, size, rotation, 0.f);
	case '7':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Left, position, size, rotation, 0.f);
	case '8':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Top, position, size, rotation, 0.f);
	case '9':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::All, position, size, rotation, 0.f);
	case 'A':
		return std::make_shared<Water>(position, size, rotation, 0.f);
	case 'B':
		return std::make_shared<Trees>(position, size, rotation, 1.f);
	case 'D':
		return nullptr;
	default:
		std::cerr << "Unknown GameObject description: " << description << std::endl;
	}

	return nullptr;
}

Level::Level(Game* pGame, const Settings& levelSettings) : m_pGame(pGame), m_levelSettings(levelSettings)
{
	m_playerState = IUnit::EUnitState::Alive; 
	m_playerLives = m_levelSettings.playerLives;
	m_nextStateTimer = std::make_pair(true, Timer());

	m_widthBlocks = m_levelSettings.levelDescription[0].length();
	m_heightBlocks = m_levelSettings.levelDescription.size();
	m_widthPixels = static_cast<unsigned int>(m_widthBlocks * BLOCK_SIZE);
	m_heightPixels = static_cast<unsigned int>(m_heightBlocks * BLOCK_SIZE);

	m_playerRespawn = { BLOCK_SIZE * (m_widthBlocks / 2 + 1),  BLOCK_SIZE / 2 };
	
	m_levelObjects.reserve(m_widthBlocks * m_heightBlocks + 4);
	unsigned int currentBottomOffset = static_cast<unsigned int>(BLOCK_SIZE) * (m_heightBlocks - 1.f / 2.f);
	for (const auto& currentRow : m_levelSettings.levelDescription)
	{
		unsigned int currentLeftOffset = BLOCK_SIZE;
		for (const auto& currentElement : currentRow)
		{
			switch (currentElement)
			{
			case 'P':
				m_playerRespawn = { currentLeftOffset, currentBottomOffset };
				m_levelObjects.emplace_back(nullptr);
				break;
			case 'E':				
				m_enemyRespawns.emplace_back(currentLeftOffset, currentBottomOffset );
				m_levelObjects.emplace_back(nullptr);
				break;
			default:
				m_levelObjects.emplace_back(createGameObjectFromDescription(currentElement, glm::vec2(currentLeftOffset, currentBottomOffset), glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.f));
				break;
			}
			
			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}

	m_enemyStates = std::vector<IUnit::EUnitState>(m_enemyRespawns.size(), IUnit::EUnitState::Alive);
	m_enemyLives = std::vector<unsigned int>(m_enemyRespawns.size(), m_levelSettings.enemyLives);
	m_enemyDeathTimers = std::vector<Timer>(m_enemyRespawns.size());

	//bottom border
	m_levelObjects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0.f), glm::vec2(BLOCK_SIZE * m_widthBlocks, BLOCK_SIZE / 2.f), 0.f, 0.f));
	//top border
	m_levelObjects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, m_heightBlocks * BLOCK_SIZE + BLOCK_SIZE / 2.f), glm::vec2(BLOCK_SIZE * m_widthBlocks, BLOCK_SIZE / 2.f), 0.f, 0.f));
	//left border
	m_levelObjects.emplace_back(std::make_shared<Border>(glm::vec2(0.f, 0.f), glm::vec2(BLOCK_SIZE, (m_heightBlocks + 1) * BLOCK_SIZE), 0.f, 0.f));
	//right border
	m_levelObjects.emplace_back(std::make_shared<Border>(glm::vec2((m_widthBlocks + 1) * BLOCK_SIZE, 0.f), glm::vec2(BLOCK_SIZE, (m_heightBlocks + 1) * BLOCK_SIZE), 0.f, 0.f));

}

void Level::render() const
{
	for (const auto& currentObject : m_levelObjects)
		if (currentObject)
			currentObject->render();

	if (m_playerLives > 0 || m_nextStateTimer.second.isRunning())
		m_pPlayer->render();
	
	for (size_t i = 0; i < m_enemies.size(); i++)
		if (m_enemyLives[i] > 0 || m_enemyDeathTimers[i].isRunning())
			m_enemies[i]->render();
}

void Level::update(const double delta)
{
	for (const auto& currentObject : m_levelObjects)
		if (currentObject)
			currentObject->update(delta);
	
	auto objectsToCheck = getObjectsInArea(m_pPlayer->getCurrentPosition() + m_pPlayer->getColliderOffset(), m_pPlayer->getCurrentPosition() + m_pPlayer->getSize() - m_pPlayer->getColliderOffset());
	bool playerInTrees = false;
	for (const auto& currentObject : objectsToCheck)
		if (currentObject->getObjectType() == IGameObject::EObjectType::Trees)
			playerInTrees = true;
	m_pPlayer->setInTrees(playerInTrees);

	if (m_playerState != m_pPlayer->getUnitState())
	{
		m_playerState = m_pPlayer->getUnitState();
		if (m_playerState == IUnit::EUnitState::Dead)
		{
			m_playerLives--;
			Physics::PhysicsEngine::removeDynamicGameObject(m_pPlayer);
		}
		else
			Physics::PhysicsEngine::addDynamicGameObject(m_pPlayer);
			
	}
		
	if (m_playerLives > 0)
		m_pPlayer->update(delta);

	for (size_t i = 0; i < m_enemies.size(); i++)
		if (m_enemyStates[i] != m_enemies[i]->getUnitState())
		{
			m_enemyStates[i] = m_enemies[i]->getUnitState();
			if (m_enemyStates[i] == IUnit::EUnitState::Dead)
			{
				m_enemyLives[i]--;
				Physics::PhysicsEngine::removeDynamicGameObject(m_enemies[i]);
			}
			else
				Physics::PhysicsEngine::addDynamicGameObject(m_enemies[i]);
				
			if (m_enemyLives[i] <= 0)
			{
				Physics::PhysicsEngine::removeDynamicGameObject(m_enemies[i]);
				m_enemyDeathTimers[i].start(2000);
			}
				
		}

	unsigned int killÑount = 0;

	for (size_t i = 0; i < m_enemies.size(); i++)
		if (m_enemyLives[i] > 0 || m_enemyDeathTimers[i].isRunning())
		{
			m_enemyDeathTimers[i].update(delta);
			m_enemies[i]->update(delta);
		}			
		else
			killÑount++;

	if (killÑount == m_enemies.size() || m_playerLives == 0)
	{
		if (m_nextStateTimer.first && !m_nextStateTimer.second.isRunning())
		{
			m_nextStateTimer.second.start(2000);
			m_nextStateTimer.first = false;
		}
		else if (m_nextStateTimer.second.isRunning())
			m_nextStateTimer.second.update(delta);
		else if(killÑount == m_enemies.size())
			m_pGame->win();
		else if(m_playerLives == 0)
			m_pGame->gameOver();

	}
}

unsigned int Level::getStateWidth() const
{
	return static_cast<unsigned int>((m_widthBlocks + 2) * BLOCK_SIZE);
}

unsigned int Level::getStateHeight() const
{
	return static_cast<unsigned int>((m_heightBlocks + 1) * BLOCK_SIZE);
}

std::vector<std::shared_ptr<IGameObject>> Level::getObjectsInArea(const glm::vec2& bottomLeft, const glm::vec2& topRight)
{
	std::vector<std::shared_ptr<IGameObject>> output;
	output.reserve(9);
	
	glm::vec2 bottomLeft_converted(std::clamp(bottomLeft.x - BLOCK_SIZE, 0.f, static_cast<float>(m_widthPixels)),
		std::clamp(m_heightPixels - bottomLeft.y + BLOCK_SIZE / 2, 0.f, static_cast<float>(m_heightPixels)));

	glm::vec2 topRight_converted(std::clamp(topRight.x - BLOCK_SIZE, 0.f, static_cast<float>(m_widthPixels)), 
		std::clamp(m_heightPixels - topRight.y + BLOCK_SIZE / 2, 0.f, static_cast<float>(m_heightPixels)));

	size_t startX = static_cast<size_t>(floor(bottomLeft_converted.x / BLOCK_SIZE));
	size_t endX = static_cast<size_t>(ceil(topRight_converted.x / BLOCK_SIZE));

	size_t startY = static_cast<size_t>(floor(topRight_converted.y / BLOCK_SIZE));
	size_t endY = static_cast<size_t>(ceil(bottomLeft_converted.y / BLOCK_SIZE));

	for (size_t currentColumn = startX; currentColumn < endX; currentColumn++)
	{
		for (size_t currentRow = startY; currentRow < endY; currentRow++)
		{
			auto& currentObject = m_levelObjects[currentRow * m_widthBlocks + currentColumn];
			if (currentObject)
			{
				output.push_back(currentObject);
			}
		}
	}

	if (endX >= m_widthBlocks)
		output.push_back(m_levelObjects[m_levelObjects.size() - 1]);
	if (startX <= 1)
		output.push_back(m_levelObjects[m_levelObjects.size() - 2]);
	if (startY <= 1)
		output.push_back(m_levelObjects[m_levelObjects.size() - 3]);
	if (endY >= m_heightBlocks)
		output.push_back(m_levelObjects[m_levelObjects.size() - 4]);

	return output;
}

void Level::processInput(const std::array<bool, 349>& keys, const std::array<bool, 349>& keysPressed)
{
	glm::vec2 vel(0);

	vel.x += (int)keys[GLFW_KEY_D];
	vel.x -= (int)keys[GLFW_KEY_A];
	vel.y += (int)keys[GLFW_KEY_W];
	vel.y -= (int)keys[GLFW_KEY_S];

	if (m_pPlayer->getOrientation() == Player::EOrientation::Top || m_pPlayer->getOrientation() == Player::EOrientation::Bottom)
	{
		bool shouldTransitionLeft = vel.y != 0 ? keysPressed[GLFW_KEY_A] : keys[GLFW_KEY_A];
		bool shouldTransitionRight = vel.y != 0 ? keysPressed[GLFW_KEY_D] : keys[GLFW_KEY_D];
		if (shouldTransitionLeft)
			m_pPlayer->setOrientation(Player::EOrientation::Left);
		else if (shouldTransitionRight)
			m_pPlayer->setOrientation(Player::EOrientation::Right);
		else if (vel.y != 0)
			m_pPlayer->setOrientation(vel.y > 0 ? Player::EOrientation::Top : Player::EOrientation::Bottom);
	}
	else
	{
		bool shouldTransitionBottom = vel.x != 0 ? keysPressed[GLFW_KEY_S] : keys[GLFW_KEY_S];
		bool shouldTransitionTop = vel.x != 0 ? keysPressed[GLFW_KEY_W] : keys[GLFW_KEY_W];
		if (shouldTransitionBottom)
			m_pPlayer->setOrientation(Player::EOrientation::Bottom);
		else if (shouldTransitionTop)
			m_pPlayer->setOrientation(Player::EOrientation::Top);
		else if (vel.x != 0)
			m_pPlayer->setOrientation(vel.x > 0 ? Player::EOrientation::Right : Player::EOrientation::Left);
	}

	if (glm::length(vel) > 0.0f)
		m_pPlayer->setVelocity(m_pPlayer->getMaxVelocity());
	else
		m_pPlayer->setVelocity(0);


	if (keys[GLFW_KEY_SPACE])
	{
		m_pPlayer->fire();
	}	
}

void Level::initLevel()
{
	m_pPlayer = std::make_shared<Player>(m_levelSettings.playerHealth, m_levelSettings.playerDamage, Player::EOrientation::Top, 0.05, m_playerRespawn, glm::vec2(BLOCK_SIZE), 0.f);
	Physics::PhysicsEngine::addDynamicGameObject(m_pPlayer);
	
	for(size_t i = 0; i < m_enemyRespawns.size(); i++)
		m_enemies.emplace_back(std::make_shared<Enemy>(m_levelSettings.enemyHealth, m_levelSettings.enemyDamage, m_pPlayer.get(), Enemy::EOrientation::Bottom, 0.05, m_enemyRespawns[i], glm::vec2(BLOCK_SIZE), 0.f));

	for (const auto& currentEnemy: m_enemies)
		Physics::PhysicsEngine::addDynamicGameObject(currentEnemy);
}
