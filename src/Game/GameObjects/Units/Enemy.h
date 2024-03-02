#pragma once

#include <glm/vec2.hpp>
#include "IUnit.h"
#include "Player.h"

class AIComponent;

class Enemy : public IUnit
{
public:

	Enemy(Player* pPlayer, const EOrientation eOrientation, const double maxVelocity, const glm::vec2& position,
		const glm::vec2& size, const float layer);

    virtual void update(const double delta) override;
	Player* getPlayer() { return m_pPlayer; }

private:
    std::unique_ptr<AIComponent> m_pAIComponent;
	Player* m_pPlayer;
};