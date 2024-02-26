#pragma once

class Enemy;

class AIComponent
{
public:
	AIComponent(Enemy* pOwner);
	void update(const double delta);

private:
	Enemy* m_pOwner;
};