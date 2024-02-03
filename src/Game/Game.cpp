#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include "Tank.h"

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Game::Game(const glm::ivec2& windowSize) : m_eCurrentGameState(EGameState::Active), m_windowSize(windowSize)
{
	m_keys.fill(false);
}

Game::~Game()
{

}

void Game::render()
{
    //ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
    if(m_pTank)
        m_pTank->render();
}

void Game::update(const uint64_t delta)
{
    //ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
    if (m_pTank)
    {
        if (m_keys[GLFW_KEY_W])
        {
            m_pTank->setOrientation(Tank::EOrientation::Top);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_A])
        {
            m_pTank->setOrientation(Tank::EOrientation::Left);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_S])
        {
            m_pTank->setOrientation(Tank::EOrientation::Bottom);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_D])
        {
            m_pTank->setOrientation(Tank::EOrientation::Right);
            m_pTank->move(true);
        }
        else
        {
            m_pTank->move(false);
        }
        m_pTank->update(delta);
    }
        
}

void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}

bool Game::init()
{
    auto pDefaultShaderprogram = ResourceManager::loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderprogram)
    {
        std::cerr << "Can't create shader program: " << "DefaultShader!" << std::endl;
        return false;
    }

    auto pSpriteShaderprogram = ResourceManager::loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
    if (!pSpriteShaderprogram)
    {
        std::cerr << "Can't create shader program: " << "SpriteShader!" << std::endl;
        return false;
    }

    auto tex = ResourceManager::loadTexture("DefaultTexture", "res/textures/map_8x8.png");
    std::vector<std::string> subTexturesnames =
    {
        "block", "topBlock",
        "bottomBlock", "leftBlock",
        "rightBlock", "topLeftBlock",
        "topRightBlock", "bottomLeftBlock",
        "bottomRightBlock", "g",
        "h", "j", "k", "l", "d",
        "water1", "water2", "water3"
    };

    auto pTextureAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_8x8.png", std::move(subTexturesnames), 8, 8);

    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "topRightBlock");
    pAnimatedSprite->setPosition(glm::vec2(300, 300));
    std::vector<std::pair<std::string, uint64_t>> waterState;
    waterState.emplace_back(std::make_pair < std::string, uint64_t>("water1", 1000000000));
    waterState.emplace_back(std::make_pair < std::string, uint64_t>("water2", 1000000000));
    waterState.emplace_back(std::make_pair < std::string, uint64_t>("water3", 1000000000));

    pAnimatedSprite->insertState("waterState", std::move(waterState));

    std::vector<std::pair<std::string, uint64_t>> eagleState;
    eagleState.emplace_back(std::make_pair < std::string, uint64_t>("block", 1000000000));
    eagleState.emplace_back(std::make_pair < std::string, uint64_t>("topBlock", 1000000000));

    pAnimatedSprite->insertState("eagleState", std::move(eagleState));

    pAnimatedSprite->setState("waterState");

    pDefaultShaderprogram->use();
    pDefaultShaderprogram->setInt("tex", 0);

    glm::mat4 modelMatrix_1 = glm::mat4(1.f);
    modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));

    glm::mat4 modelMatrix_2 = glm::mat4(1.f);
    modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

    pDefaultShaderprogram->setMatrix4("projectionMatrix", projectionMatrix);

    pSpriteShaderprogram->use();
    pSpriteShaderprogram->setInt("tex", 0);
    pSpriteShaderprogram->setMatrix4("projectionMatrix", projectionMatrix);


    std::vector<std::string> tanksSubTexturesnames =
    {
        "tankTop1", "tankTop2",
        "tankLeft1", "tankLeft2",
        "tankBottom1", "tankBottom2",
        "tankRight1", "tankRight2",
    };

    auto pTanksTextureAtlas = ResourceManager::loadTextureAtlas("TanksTextureAtlas", "res/textures/tanks.png", std::move(tanksSubTexturesnames), 16, 16);
    auto pTanksAnimatedSprite = ResourceManager::loadAnimatedSprite("TanksAnimatedSprite", "TanksTextureAtlas", "SpriteShader", 100, 100, "tankTop1");
    
    std::vector<std::pair<std::string, uint64_t>> tankTopState;
    tankTopState.emplace_back(std::make_pair < std::string, uint64_t>("tankTop1", 500000000));
    tankTopState.emplace_back(std::make_pair < std::string, uint64_t>("tankTop2", 500000000));

    std::vector<std::pair<std::string, uint64_t>> tankLeftState;
    tankLeftState.emplace_back(std::make_pair < std::string, uint64_t>("tankLeft1", 500000000));
    tankLeftState.emplace_back(std::make_pair < std::string, uint64_t>("tankLeft2", 500000000));

    std::vector<std::pair<std::string, uint64_t>> tankBottomState;
    tankBottomState.emplace_back(std::make_pair < std::string, uint64_t>("tankBottom1", 500000000));
    tankBottomState.emplace_back(std::make_pair < std::string, uint64_t>("tankBottom2", 500000000));

    std::vector<std::pair<std::string, uint64_t>> tankRightState;
    tankRightState.emplace_back(std::make_pair < std::string, uint64_t>("tankRight1", 500000000));
    tankRightState.emplace_back(std::make_pair < std::string, uint64_t>("tankRight2", 500000000));


    pTanksAnimatedSprite->insertState("tankTopState", std::move(tankTopState));
    pTanksAnimatedSprite->insertState("tankLeftState", std::move(tankLeftState));
    pTanksAnimatedSprite->insertState("tankBottomState", std::move(tankBottomState));
    pTanksAnimatedSprite->insertState("tankRightState", std::move(tankRightState));

    pTanksAnimatedSprite->setState("tankTopState");

    m_pTank = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f, glm::vec2(100.f, 100.f));



    return true;
}