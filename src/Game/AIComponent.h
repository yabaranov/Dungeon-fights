#pragma once

class Enemy;

class AIComponent
{
public:
	AIComponent(Enemy* pParentTank);
	void update(const double delta);

private:
	Enemy* m_pParentTank;
};