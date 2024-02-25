#include "StartScreen.h"

#include <iostream>
#include <array>

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"
#include "../Game.h"

#include <GLFW/glfw3.h>

StartScreen::StartScreen(Game* pGame) : m_pGame(pGame),
    m_currentMenuSelection(0),
    m_keyReleased(true),
    m_titleSprite(std::make_pair(ResourceManager::getSprite("title"), glm::vec2(6 * BLOCK_SIZE, STARTSCREEN_HEIGHT - 13 * BLOCK_SIZE))),
    m_menuSprite(std::make_pair(ResourceManager::getSprite("menu"), glm::vec2(11 * BLOCK_SIZE, STARTSCREEN_HEIGHT - 9 * BLOCK_SIZE - MENU_HEIGHT - 5 * BLOCK_SIZE))),
    m_playerSprite(std::make_pair(ResourceManager::getSprite("player"), glm::vec2(8 * BLOCK_SIZE, m_menuSprite.second.y + 6 * BLOCK_SIZE - m_currentMenuSelection * 2 * BLOCK_SIZE)))
{
}

void StartScreen::render() const
{
    m_titleSprite.first->render(m_titleSprite.second, glm::vec2(TITLE_WIDTH, TITLE_HEIGHT), 0.f);
    m_menuSprite.first->render(m_menuSprite.second, glm::vec2(MENU_WIDTH, MENU_HEIGHT), 0.f);
    m_playerSprite.first->render(glm::vec2(m_playerSprite.second.x, m_playerSprite.second.y - m_currentMenuSelection * 2 * BLOCK_SIZE), glm::vec2(BLOCK_SIZE * 2), 0.f, 0.f);
}

void StartScreen::update(const double delta)
{
}

void StartScreen::processInput(const std::array<bool, 349>& keys)
{
    if (!keys[GLFW_KEY_W] && !keys[GLFW_KEY_S])
    {
        m_keyReleased = true;
    }
    if (m_keyReleased)
    {
        if (keys[GLFW_KEY_W])
        {
            m_keyReleased = false;
            m_currentMenuSelection--;
            if (m_currentMenuSelection < 0)
                m_currentMenuSelection = 2;
        }
        else if (keys[GLFW_KEY_S])
        {
            m_keyReleased = false;
            m_currentMenuSelection++;
            if (m_currentMenuSelection > 2)
                m_currentMenuSelection = 0;
        }
    }
    
    if (keys[GLFW_KEY_ENTER])
    {
        switch (m_currentMenuSelection)
        {
        case 0:
            m_pGame->startNewLevel(0);
            break;
        default:
            break;

        }
    }
}

unsigned int StartScreen::getStateWidth() const
{
    return STARTSCREEN_WIDTH;
}

unsigned int StartScreen::getStateHeight() const
{
    return STARTSCREEN_HEIGHT;
}



