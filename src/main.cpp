#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
    void update(float dt) {
        PlayLayer::update(dt);

        if (!m_player1 || m_isTestMode || m_player1->m_isDead || m_player1->m_isAutomation) return;

        processBotLogic(m_player1, dt);

        if (m_gameState.m_isDualMode && m_player2) {
            processBotLogic(m_player2, dt);
        }
    }

    void processBotLogic(PlayerObject* player, float dt) {
        auto playerPos = player->getPosition();
        float velocityX = player->m_xVelocity;
        
        float detectRange = (velocityX * 0.45f) + (velocityX * dt);
        if (detectRange < 25.0f) detectRange = 25.0f;

        bool shouldJump = false;
        float searchMaxX = playerPos.x + detectRange;

        auto objects = m_objects;
        int count = objects->count();

        for (int i = 0; i < count; ++i) {
            auto obj = static_cast<GameObject*>(objects->objectAtIndex(i));
            
            if (!obj || obj->m_isDead || !obj->m_isHazard) continue;

            float objX = obj->getPositionX();
            
            if (objX < playerPos.x - 5.0f || objX > searchMaxX) continue;

            float objY = obj->getPositionY();
            float yDist = std::abs(playerPos.y - objY);

            if (yDist < 35.0f) {
                if (player->m_isShip || player->m_isBird || player->m_isDart) {
                    if (playerPos.y < objY + 2.0f) {
                        shouldJump = true;
                    }
                } else if (player->m_isBall || player->m_isSwing) {
                    shouldJump = true;
                } else if (player->m_isGrounded) {
                    shouldJump = true;
                }
                
                if (shouldJump) break;
            }
        }

        if (shouldJump) {
            player->pushButton(PlayerButton::Jump);
        } else {
            player->releaseButton(PlayerButton::Jump);
        }
    }
};
