#pragma once

#include "IGameObject.h"
#include "../../Renderer/SpriteAnimator.h"
#include "../../System/Timer.h"

#include <array>
#include <memory>

namespace RenderEngine {
    class Sprite;
}

class Bullet : public IGameObject {
public:

    Bullet(const double velocity,
        const glm::vec2& position,
        const glm::vec2& size,
        const glm::vec2& explosionSize,
        const float layer);
    virtual void render() const override;
    virtual void update(const double delta) override;
    bool isActive() const { return m_isActive; }
    void fire(const glm::vec2& position, const glm::vec2& direction);
    virtual bool collides(const EObjectType objectType) override;

private:
    glm::vec2 m_explosionSize;
    glm::vec2 m_explosionOffset;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_explosion;
    RenderEngine::SpriteAnimator m_spriteAnimator_explosion;
    Timer m_explosionTimer;
    double m_maxVelocity;
    bool m_isActive;
    bool m_isExplosion;
};