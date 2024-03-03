#include "AIComponent.h"
#include "GameObjects/Units/Enemy.h"

#include <random>

#include <glm/geometric.hpp>
AIComponent::AIComponent(Enemy* pOwner) : m_pOwner(pOwner)
{
	m_moveTimer.setCallback([&]()
		{
			setRandomOrientation();
			std::random_device rd;
			std::mt19937 gen(rd());
			std::normal_distribution dist(0.0, 4.0);
			m_moveTimer.start(1000 * dist(gen));
		}
	);

	m_moveTimer.start(1000);

	m_hitTimer.setCallback([&]()
		{
			m_pOwner->setHit(true);
			std::random_device rd;
			std::mt19937 gen(rd());
			std::normal_distribution dist(0.0, 2.0);
			m_hitTimer.start(1000 * dist(gen));
		}
	);

	m_hitTimer.start(1000);

}

void AIComponent::update(const double delta)
{	
	if (m_pOwner->getPlayer()->getUnitState() == IUnit::EUnitState::Alive && 
		glm::distance(m_pOwner->getPlayer()->getCurrentPosition(), m_pOwner->getCurrentPosition()) <= 5 * BLOCK_SIZE)
	{
		if (m_pOwner->hasHit() && std::abs(m_pOwner->getPlayer()->getCurrentPosition().x - m_pOwner->getCurrentPosition().x) <= BLOCK_SIZE / 4)
		{		
			if (m_pOwner->getCurrentPosition().y < m_pOwner->getPlayer()->getCurrentPosition().y)
				m_pOwner->setOrientation(IUnit::EOrientation::Top);
			else
				m_pOwner->setOrientation(IUnit::EOrientation::Bottom);
							
			m_pOwner->setVelocity(0.0);
			m_pOwner->fire();						
		}
		else if (m_pOwner->hasHit() && std::abs(m_pOwner->getPlayer()->getCurrentPosition().y - m_pOwner->getCurrentPosition().y) <= BLOCK_SIZE / 4)
		{			
			if (m_pOwner->getCurrentPosition().x < m_pOwner->getPlayer()->getCurrentPosition().x)
				m_pOwner->setOrientation(IUnit::EOrientation::Right);
			else
				m_pOwner->setOrientation(IUnit::EOrientation::Left);

			m_pOwner->setVelocity(0.0);
			m_pOwner->fire();
		}
		else
		{		
			m_hitTimer.update(delta);
			m_pOwner->setVelocity(m_pOwner->getMaxVelocity());
			m_moveTimer.update(delta);
		}
	}
	else
	{		
		m_hitTimer.update(delta);
		m_pOwner->setVelocity(m_pOwner->getMaxVelocity());
		m_moveTimer.update(delta);
	}
}

void AIComponent::setRandomOrientation()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 3);

	m_pOwner->setOrientation(static_cast<IUnit::EOrientation>(dist(gen)));
}

