#include "GameOver.h"

#include <iostream>
#include <array>

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"
#include "../Game.h"

#include <GLFW/glfw3.h>

GameOver::GameOver(Game* pGame) : m_pGame(pGame),
    m_Sprite(std::make_pair(ResourceManager::getSprite("game_over"), glm::vec2(6 * BLOCK_SIZE, SCREEN_HEIGHT - 16 * BLOCK_SIZE)))
{
}

void GameOver::render() const
{
    m_Sprite.first->render(m_Sprite.second, glm::vec2(GAME_OVER_WIDTH, GAME_OVER_HEIGHT), 0.f);
}

void GameOver::update(const double delta)
{
}

void GameOver::processInput(const std::array<bool, 349>& keys)
{    
    if (keys[GLFW_KEY_ENTER])    
        m_pGame->startScreen();    
}

unsigned int GameOver::getStateWidth() const
{
    return SCREEN_WIDTH;
}

unsigned int GameOver::getStateHeight() const
{
    return SCREEN_HEIGHT;
}



