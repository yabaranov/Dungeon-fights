#include "Blood.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

#include <algorithm>

BloodUnit::BloodUnit(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) :
	IGameObject(IGameObject::EObjectType::Water, position, size, rotation, layer),
	m_pSprite(ResourceManager::getSprite("blood")),
	m_pSpriteAnimator(m_pSprite), 
	m_isActive(true)
{	
	m_timer.setCallback([&]()
		{
			m_isActive = false;
		}
	);

	m_timer.start(m_pSpriteAnimator.getTotalDuration());

}

void BloodUnit::render() const
{
	m_pSprite->render(m_position, m_size, m_rotation, m_layer, m_pSpriteAnimator.getCurrentFrame());	
}

void BloodUnit::update(const double delta)
{	
	m_pSpriteAnimator.update(delta);
	m_timer.update(delta);	
}

void Blood::render() const
{
	for (const auto& currentBlood : m_blood)
		if (currentBlood->isActive())
			currentBlood->render();
}

void Blood::update(const double delta)
{
	for (const auto& currentBlood : m_blood)
		if (currentBlood->isActive())
			currentBlood->update(delta);

	auto newEnd = std::remove_if(m_blood.begin(), m_blood.end(), [](const auto& bloodUnit)
		{
			return !bloodUnit->isActive();
		});

	m_blood.erase(newEnd, m_blood.end());
}