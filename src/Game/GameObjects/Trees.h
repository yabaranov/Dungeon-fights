#pragma once

#include "IGameObject.h"
#include <array>
#include <memory>

namespace RenderEngine
{
	class Sprite;
}

class Trees : public IGameObject
{
public:
	
	enum class EBlockLocation : uint8_t
	{
		TopLeft,
		TopRight, 
		BottomLeft,
		BottomRight
	};

	Trees(const glm::vec2& position, const glm::vec2& size, const float rotation);
	
	void render() const override;
		
private:
	void renderBlock(const EBlockLocation eBlockLocation) const;
	std::shared_ptr<RenderEngine::Sprite> m_sprite;
	std::array<glm::vec2, 4> m_blockOffsets;
};