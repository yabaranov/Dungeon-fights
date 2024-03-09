#include "StartScreen.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"
#include "../Game.h"

#include <GLFW/glfw3.h>

StartScreen::StartScreen(Game* pGame) : m_pGame(pGame),
    m_currentMenuSelection(0),
    m_titleSprite(std::make_pair(ResourceManager::getSprite("title"), glm::vec2(5 * BLOCK_SIZE, SCREEN_HEIGHT - 13 * BLOCK_SIZE))),
    m_menuSprite(std::make_pair(ResourceManager::getSprite("menu"), glm::vec2(10 * BLOCK_SIZE, SCREEN_HEIGHT - 9 * BLOCK_SIZE - TEXTURE_HEIGHT - 5 * BLOCK_SIZE))),
    m_playerSprite(std::make_pair(ResourceManager::getSprite("player"), glm::vec2(6 * BLOCK_SIZE, m_menuSprite.second.y + 6.5f * BLOCK_SIZE - m_currentMenuSelection * 2 * BLOCK_SIZE)))
{
}

void StartScreen::render() const
{
    m_titleSprite.first->render(m_titleSprite.second, glm::vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT), 0.f);
    m_menuSprite.first->render(m_menuSprite.second, glm::vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT), 0.f);
    m_playerSprite.first->render(glm::vec2(m_playerSprite.second.x, m_playerSprite.second.y - m_currentMenuSelection * 2 * BLOCK_SIZE), glm::vec2(BLOCK_SIZE * 2), 0.f, 0.f);
}

void StartScreen::processInput(const std::array<bool, 349>& keys, const std::array<bool, 349>& keysPressed)
{    
   if (keysPressed[GLFW_KEY_W])
   {      
       m_currentMenuSelection--;
       if (m_currentMenuSelection < 0)
           m_currentMenuSelection = 1;
   }
   else if (keysPressed[GLFW_KEY_S])
   {
       m_currentMenuSelection++;
       if (m_currentMenuSelection > 1)
           m_currentMenuSelection = 0;
   }
   else if (keysPressed[GLFW_KEY_ENTER])
   {
       switch (m_currentMenuSelection)
       {
       case 0:
           m_pGame->level(0);
           break;
       default:
           break;

       }
   }
    
}

unsigned int StartScreen::getStateWidth() const
{
    return SCREEN_WIDTH;
}

unsigned int StartScreen::getStateHeight() const
{
    return SCREEN_HEIGHT;
}