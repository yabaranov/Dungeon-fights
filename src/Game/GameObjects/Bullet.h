#pragma once

#include "IGameObject.h"
#include "Units/IUnit.h"
#include "../../Renderer/SpriteAnimator.h"
#include "../../System/Timer.h"

#include <memory>

namespace RenderEngine {
    class Sprite;
}

class Bullet : public IGameObject {
public:

    Bullet(IUnit* pOwner, const double velocity,
        const glm::vec2& position,
        const glm::vec2& size,
        const glm::vec2& explosionSize,
        const float layer);
    virtual void render() const override;
    virtual void update(const double delta) override;
    bool isActive() const { return m_isActive; }
    void fire(const glm::vec2& position, const glm::vec2& direction);
    IUnit* getOwner() const { return m_pOwner; }

private:
    IUnit* m_pOwner;
    glm::vec2 colliderOffset;
    glm::vec2 m_explosionSize;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_explosion;
    RenderEngine::SpriteAnimator m_spriteAnimator_explosion;
    Timer m_explosionTimer;
    double m_maxVelocity;
    bool m_isActive;
    bool m_isExplosion;
};