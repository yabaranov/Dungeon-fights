#include "AIComponent.h"
#include "GameObjects/Units/Enemy.h"

AIComponent::AIComponent(Enemy* pOwner) : m_pOwner(pOwner)
{
}

void AIComponent::update(const double delta)
{	
	m_pOwner->fire();
}
