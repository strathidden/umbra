#include "Enemy.h"
#include "../graphics/SpriteSheet.h"
#include "../core/Engine.h"
#include "../game/Game.h"

Enemy::Enemy()
{
    m_physicsBody = addComponent<PhysicsBody>(glm::vec2(500.0f, 100.0f), glm::vec2(60.0f, 80.0f));

    m_physicsBody->collider.layer = 3;
    m_startPosition = m_physicsBody->position;

    m_animator = addComponent<Animator>();

    auto spriteSheet = std::make_unique<SpriteSheet>();
    if (spriteSheet->loadFromFile("assets/enemy_sheet.png", {60, 80}))
    {
        Animation patrolAnim;
        for (int i = 0; i < 4; i++) {
            patrolAnim.addFrame(spriteSheet->createFrame(i, 0.2f));
        }
        patrolAnim.setLooping(true);
        m_animator->addAnimation("patrol", std::move(patrolAnim));
        
        Animation chaseAnim;
        for (int i = 4; i < 8; i++) {
            chaseAnim.addFrame(spriteSheet->createFrame(i, 0.1f));
        }
        chaseAnim.setLooping(true);
        m_animator->addAnimation("chase", std::move(chaseAnim));
        
        Animation attackAnim;
        attackAnim.addFrame(spriteSheet->createFrame(8, 0.15f));
        attackAnim.addFrame(spriteSheet->createFrame(9, 0.15f));
        attackAnim.addFrame(spriteSheet->createFrame(10, 0.15f));
        attackAnim.setLooping(false);
        m_animator->addAnimation("attack", std::move(attackAnim));
    }
    
    m_animator->play("patrol");
}

void Enemy::update(float deltaTime)
{
    auto engine = Engine::getInstance();
    auto& game = static_cast<Game&>(*engine);

    Entity::update(deltaTime);

    position = m_physicsBody->position;

    switch (m_state)
    {
        case State::PATROLLING:
        {
            float patrolOffset = position.x - m_startPosition.x;
            if (std::abs(patrolOffset) > m_patrolDistance)
            {
                m_physicsBody->velocity.x = -m_physicsBody->velocity.x;
            }

            auto player = game.getPlayer();
            if (player && glm::distance(position, player->position) < m_detectionRange)
            {
                m_state = State::CHASING;
                m_animator->play("chase");
            }
            break;
        }

        case State::CHASING:
        {
            auto player = game.getPlayer();
            if (player)
            {
                float direction = (player->position.x > position.x) ? 1.0f : -1.0f;
                m_physicsBody->velocity.x = direction * 150.0f;

                if (glm::distance(position, player->position) < 100.0f)
                {
                    m_state = State::ATTACKING;
                    m_animator->play("attack");
                }
            }
            else
            {
                m_state = State::PATROLLING;
                m_animator->play("patrol");
            }
            break;
        }

        case State::ATTACKING:
        {
            if (m_animator->getCurrentAnimation()->isFinished())
            {
                m_state = State::CHASING;
                m_animator->play("chase");
            }
            break;
        }
    }
}
