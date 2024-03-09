#include "Win.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"
#include "../Game.h"

#include <GLFW/glfw3.h>

Win::Win(Game* pGame) : m_pGame(pGame),
    m_Sprite(std::make_pair(ResourceManager::getSprite("win"), glm::vec2(5 * BLOCK_SIZE, SCREEN_HEIGHT - 16 * BLOCK_SIZE)))
{
}

void Win::render() const
{
    m_Sprite.first->render(m_Sprite.second, glm::vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT), 0.f);
}

void Win::processInput(const std::array<bool, 349>& keys, const std::array<bool, 349>&)
{    
    if (keys[GLFW_KEY_ENTER])
        if (m_pGame->getCurrentLevel() + 1 < m_pGame->getMaxLevel())
            m_pGame->level(m_pGame->getCurrentLevel() + 1);
        else
            m_pGame->startScreen();    
}

unsigned int Win::getStateWidth() const
{
    return SCREEN_WIDTH;
}

unsigned int Win::getStateHeight() const
{
    return SCREEN_HEIGHT;
}



