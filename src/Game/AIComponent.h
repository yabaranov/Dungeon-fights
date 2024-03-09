#pragma once

#include "../System/Timer.h"

class Enemy;
class Player;

class AIComponent
{
public:
	AIComponent(Enemy* pOwner, const Player* pPlayer);
	void update(const double delta);
	void setRandomOrientation();

private:
	static constexpr unsigned int BLOCK_SIZE = 16;
	Enemy* m_pOwner;
	const Player* m_pPlayer;
	Timer m_moveTimer;
	Timer m_hitTimer;
};