#include "AIComponent.h"
#include "GameObjects/Units/Enemy.h"

AIComponent::AIComponent(Enemy* pParentTank) : m_pParentTank(pParentTank)
{
}

void AIComponent::update(const double delta)
{
	m_pParentTank->fire();

}
