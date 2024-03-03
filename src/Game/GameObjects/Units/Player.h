#pragma once

#include <glm/vec2.hpp>
#include "IUnit.h"

class Player : public IUnit
{
public:
	Player(const EOrientation eOrientation, const double maxVelocity, const glm::vec2& position,
		const glm::vec2& size, const float layer);
	bool inTrees() const { return m_inTrees; }
	void setInTrees(bool inTrees) { m_inTrees = inTrees; }

private:
	bool m_inTrees;
};