#include "AIComponent.h"
#include "GameObjects/Units/Enemy.h"

AIComponent::AIComponent(Enemy* pOwner) : m_pOwner(pOwner)
{
}

void AIComponent::update(const double delta)
{
	if(m_pOwner->getUnitState() == IUnit::EUnitState::Alive)
		m_pOwner->fire();
}
