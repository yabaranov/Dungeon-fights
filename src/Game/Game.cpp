#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Renderer.h"

#include "GameStates/Level.h"
#include "GameStates/StartScreen.h"
#include "GameStates/GameOver.h"
#include "GameStates/Win.h"
#include "../Physics/PhysicsEngine.h"

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Game::Game(const glm::uvec2& windowSize) : m_windowSize(windowSize), m_currentLevel(0), m_maxLevel(0)
{
	m_keys.fill(false);
    m_keysPressed.fill(false);
}

Game::~Game()
{
}

void Game::render() const
{
    m_pCurrentGameState->render(); 
}

void Game::update(const double delta)
{  
    m_pCurrentGameState->processInput(m_keys, m_keysPressed);
    m_keysPressed.fill(false);
    m_pCurrentGameState->update(delta);
}

void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}

void Game::setKeyPressed(const int key, const int action)
{
    if (action != GLFW_REPEAT)
        m_keysPressed[key] = action;
}

bool Game::init()
{
    ResourceManager::loadJSONResources("res/resources.json");
    
    m_pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    if (!m_pSpriteShaderProgram)
    {
        std::cerr << "Can't find shader program: " << "spriteShader" << std::endl;
        return false;
    }
    m_pSpriteShaderProgram->use();
    m_pSpriteShaderProgram->setInt("tex", 0);
    ResourceManager::loadJSONLevelSettings("res/levelSettings.json");

    m_maxLevel = ResourceManager::getLevelSettings().size();
    startScreen();
    return true;
}

size_t Game::getCurrentWidth() const
{
    return m_pCurrentGameState->getStateWidth();
}

size_t Game::getCurrentHeight() const
{
    return m_pCurrentGameState->getStateHeight();
}

void Game::level(const size_t level)
{
    m_currentLevel = level;
    auto pLevel = std::make_shared<Level>(this, ResourceManager::getLevelSettings()[level]);
    m_pCurrentGameState = pLevel;
    Physics::PhysicsEngine::setLevel(pLevel);
    updateViewport();
}

void Game::gameOver()
{
    m_pCurrentGameState = std::make_shared<GameOver>(this);
    Physics::PhysicsEngine::terminate();
    updateViewport();
}

void Game::win()
{
    m_pCurrentGameState = std::make_shared<Win>(this);
    Physics::PhysicsEngine::terminate();
    updateViewport();
}

void Game::startScreen()
{
    m_pCurrentGameState = std::make_shared<StartScreen>(this);
    updateViewport();
}


void Game::setWindowSize(const glm::uvec2& windowSize)
{
    m_windowSize = windowSize;
    updateViewport(); 
}

void Game::updateViewport()
{
    const float mapAspectRatio = static_cast<float>(getCurrentWidth()) / getCurrentHeight();
    unsigned int viewPortWidth = m_windowSize.x;
    unsigned int viewPortHeight = m_windowSize.y;
    unsigned int viewPortLeftOffset = 0;
    unsigned int viewPortBottomOffset = 0;

    if (static_cast<float>(m_windowSize.x) / m_windowSize.y > mapAspectRatio)
    {
        viewPortWidth = static_cast<unsigned int>(m_windowSize.y * mapAspectRatio);
        viewPortLeftOffset = (m_windowSize.x - viewPortWidth) / 2;
    }
    else
    {
        viewPortHeight = static_cast<unsigned int>(m_windowSize.x / mapAspectRatio);
        viewPortBottomOffset = (m_windowSize.y - viewPortHeight) / 2;
    }

    RenderEngine::Renderer::setViewport(viewPortWidth, viewPortHeight, viewPortLeftOffset, viewPortBottomOffset);

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(getCurrentWidth()), 0.f, static_cast<float>(getCurrentHeight()), -100.f, 100.f);
    m_pSpriteShaderProgram->setMatrix4("projectionMatrix", projectionMatrix);
}
