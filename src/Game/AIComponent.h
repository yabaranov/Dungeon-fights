#pragma once

#include "../System/Timer.h"

class Enemy;

class AIComponent
{
public:
	AIComponent(Enemy* pOwner);
	void update(const double delta);
	void setRandomOrientation();

private:
	static constexpr unsigned int BLOCK_SIZE = 16;
	Enemy* m_pOwner;
	Timer m_moveTimer;
	Timer m_hitTimer;

};