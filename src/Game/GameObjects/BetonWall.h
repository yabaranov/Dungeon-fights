#pragma once

#include "IGameObject.h"
#include <array>
#include <memory>

namespace RenderEngine
{
	class Sprite;
}

class BetonWall : public IGameObject
{
public:
	enum class EBetonWallType : uint8_t
	{
		All, 
		Top, 
		Bottom,
		Left, 
		Right,
	};

	enum class EBlockState : uint8_t
	{
		Enabled,
		Destroyed
	};

	enum class EBlockLocation : uint8_t
	{
		TopLeft,
		TopRight, 
		BottomLeft,
		BottomRight
	};

	BetonWall(const EBetonWallType eBetonWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	
	virtual void render() const override;
	
private:
	void renderBlock(const EBlockLocation eBlockLocation) const;
	std::array<EBlockState, 4> m_eCurrentBlockState;
	std::shared_ptr<RenderEngine::Sprite> m_sprite;
	std::array<glm::vec2, 4> m_blockOffsets;
};