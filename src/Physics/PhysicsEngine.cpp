#include "PhysicsEngine.h"

#include "../Game/GameObjects/IGameObject.h"
#include "../Game/GameStates/Level.h"

#include <iostream>

namespace Physics {

    std::unordered_set<std::shared_ptr<IGameObject>> PhysicsEngine::m_dynamicObjects;
    std::shared_ptr<Level> PhysicsEngine::m_pCurrentLevel;

    void PhysicsEngine::init()
    {

    }

    void PhysicsEngine::terminate()
    {
        m_dynamicObjects.clear();
        m_pCurrentLevel.reset();
    }

    void PhysicsEngine::setLevel(std::shared_ptr<Level> pLevel)
    {
        m_pCurrentLevel.swap(pLevel);
        m_dynamicObjects.clear();
        m_pCurrentLevel->initLevel();
    }

    void PhysicsEngine::update(const double delta)
    {
        calculateTargetPositions(delta);
        
        for (auto it1 = m_dynamicObjects.begin(); it1 != m_dynamicObjects.end();)
        {
            auto pObject1 = *it1;
            for (auto it2 = ++it1; it2 != m_dynamicObjects.end(); it2++)
            {
                auto pObject2 = *it2;
                if (intersectionReactions(pObject1, pObject1->getTargetPosition(), pObject2, pObject2->getTargetPosition()))
                {
                    pObject1->getTargetPosition() = pObject1->getCurrentPosition();
                    pObject2->getTargetPosition() = pObject2->getCurrentPosition();
                }
            }
        }

        updatePositions();        
    }

    void PhysicsEngine::addDynamicGameObject(std::shared_ptr<IGameObject> pGameObject)
    {
        m_dynamicObjects.insert(std::move(pGameObject));
    }

    void PhysicsEngine::removeDynamicGameObject(std::shared_ptr<IGameObject> pGameObject)
    {
        m_dynamicObjects.erase(pGameObject);
    }


    bool PhysicsEngine::hasCollidersIntersection(const Collider& collider1, const glm::vec2& position1,
        const Collider& collider2, const glm::vec2& position2)
    {
        const glm::vec2 collider1_bottomLeft_world = collider1.boundingBox.bottomLeft + position1;
        const glm::vec2 collider1_topRight_world = collider1.boundingBox.topRight + position1;

        const glm::vec2 collider2_bottomLeft_world = collider2.boundingBox.bottomLeft + position2;
        const glm::vec2 collider2_topRight_world = collider2.boundingBox.topRight + position2;

        if (collider1_bottomLeft_world.x >= collider2_topRight_world.x)
        {
            return false;
        }
        if (collider1_topRight_world.x <= collider2_bottomLeft_world.x)
        {
            return false;
        }

        if (collider1_bottomLeft_world.y >= collider2_topRight_world.y)
        {
            return false;
        }
        if (collider1_topRight_world.y <= collider2_bottomLeft_world.y)
        {
            return false;
        }

        return true;
    }

    bool PhysicsEngine::intersectionReactions(const std::shared_ptr<IGameObject>& pObject1, const glm::vec2& position1, const std::shared_ptr<IGameObject>& pObject2, const glm::vec2& position2)
    {
        ECollisionDirection collisionDirectionObject_1 = ECollisionDirection::Right;
        if (pObject1->getCurrentDirection().x < 0) collisionDirectionObject_1 = ECollisionDirection::Left;
        else if (pObject1->getCurrentDirection().y > 0) collisionDirectionObject_1 = ECollisionDirection::Top;
        else if (pObject1->getCurrentDirection().y < 0) collisionDirectionObject_1 = ECollisionDirection::Bottom;

        ECollisionDirection collisionDirectionObject_2 = ECollisionDirection::Left;
        if (pObject1->getCurrentDirection().x < 0) collisionDirectionObject_2 = ECollisionDirection::Right;
        else if (pObject1->getCurrentDirection().y > 0) collisionDirectionObject_2 = ECollisionDirection::Bottom;
        else if (pObject1->getCurrentDirection().y < 0) collisionDirectionObject_2 = ECollisionDirection::Top;

        bool hasCollision = false;
        if (pObject2->collides(pObject1->getObjectType()) && pObject1->collides(pObject2->getObjectType()))
            for (const auto& currentObjectCollider : pObject1->getColliders())      
                for (const auto& otherObjectCollider : pObject2->getColliders())            
                    if (otherObjectCollider.isActive && currentObjectCollider.isActive && hasCollidersIntersection(currentObjectCollider, position1, otherObjectCollider, position2))
                    {
                        hasCollision = true;
                        if (currentObjectCollider.onCollisionCallback)                        
                            currentObjectCollider.onCollisionCallback(*pObject2, collisionDirectionObject_1);                      
                        if (otherObjectCollider.onCollisionCallback)                        
                            otherObjectCollider.onCollisionCallback(*pObject1, collisionDirectionObject_2);                        
                    }
                                      
        return hasCollision;
    }
    
    void PhysicsEngine::calculateTargetPositions(const double delta)
    {
        for (auto& currentDynamicObject : m_dynamicObjects)
        {
            if (currentDynamicObject->getCurrentVelocity() > 0)
            {
                if (currentDynamicObject->getCurrentDirection().x != 0.f)               
                    currentDynamicObject->getTargetPosition() = glm::vec2(currentDynamicObject->getCurrentPosition().x, static_cast<unsigned int>(currentDynamicObject->getCurrentPosition().y / 4.f + 0.5f) * 4.f);               
                else if (currentDynamicObject->getCurrentDirection().y != 0.f)              
                    currentDynamicObject->getTargetPosition() = glm::vec2(static_cast<unsigned int>(currentDynamicObject->getCurrentPosition().x / 4.f + 0.5f) * 4.f, currentDynamicObject->getCurrentPosition().y);
                
                const auto newPosition = currentDynamicObject->getTargetPosition() + currentDynamicObject->getCurrentDirection() * static_cast<float>(currentDynamicObject->getCurrentVelocity() * delta);
                std::vector<std::shared_ptr<IGameObject>> objectsToCheck = m_pCurrentLevel->getObjectsInArea(newPosition, newPosition + currentDynamicObject->getSize());
                
                bool hasCollision = false;

                for (const auto& currentObjectToCheck : objectsToCheck)                                 
                    if (intersectionReactions(currentDynamicObject, newPosition, currentObjectToCheck, currentObjectToCheck->getCurrentPosition()))
                        hasCollision = true;                    
                
                if (!hasCollision)               
                    currentDynamicObject->getTargetPosition() = newPosition;               
                else
                {
                    if (currentDynamicObject->getCurrentDirection().x != 0.f)               
                        currentDynamicObject->getTargetPosition() = glm::vec2(static_cast<unsigned int>(currentDynamicObject->getTargetPosition().x / 4.f + 0.5f) * 4.f, currentDynamicObject->getTargetPosition().y);                
                    else if (currentDynamicObject->getCurrentDirection().y != 0.f)                   
                        currentDynamicObject->getTargetPosition() = glm::vec2(currentDynamicObject->getTargetPosition().x, static_cast<unsigned int>(currentDynamicObject->getTargetPosition().y / 4.f + 0.5f) * 4.f);                   
                }
            }
        }

    }

    void PhysicsEngine::updatePositions()
    {
        for (auto& currentDynamicObject : m_dynamicObjects)
            currentDynamicObject->getCurrentPosition() = currentDynamicObject->getTargetPosition();
    }
}