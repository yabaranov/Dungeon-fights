#pragma once

#include <glm/vec2.hpp>
#include "IUnit.h"

class AIComponent;

class Enemy : public IUnit
{
public:

	Enemy(const EOrientation eOrientation, const double maxVelocity, const glm::vec2& position, 
		const glm::vec2& size, const float layer);

    virtual void update(const double delta) override;

private:
    std::unique_ptr<AIComponent> m_pAIComponent;
};