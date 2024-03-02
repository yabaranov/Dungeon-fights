#include "Blood.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

Blood::Blood(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) :
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

void Blood::render() const
{
	if (m_isActive)
	{
		m_pSprite->render(m_position, m_size, m_rotation, m_layer, m_pSpriteAnimator.getCurrentFrame());
	}
}

void Blood::update(const double delta)
{
	if (m_isActive)
	{
		m_pSpriteAnimator.update(delta);
		m_timer.update(delta);
	}
}
