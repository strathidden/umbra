#include "Player.h"
#include "../graphics/SpriteSheet.h"
#include "../core/InputManager.h"

Player::Player()
{
    m_physicsBody = addComponent<PhysicsBody>(glm::vec2(100.0f, 100.0f), glm::vec2(50.0f, 100.0f));
    m_physicsBody->collider.layer = 1;

    m_animator = addComponent<Animator>();

    auto spriteSheet = std::make_unique<SpriteSheet>();
    if (spriteSheet->loadFromFile("assets/player_sheet.png", {50, 100}))
    {
        Animation idleAnim;
        for (int i = 0; i < 4; i++)
        {
            idleAnim.addFrame(spriteSheet->createFrame(i, 0.15f));
        }
        idleAnim.setLooping(true);
        m_animator->addAnimation("idle", std::move(idleAnim));

        Animation runAnim;
        for (int i = 4; i < 8; i++)
        {
            runAnim.addFrame(spriteSheet->createFrame(i, 0.1f));
        }
        runAnim.setLooping(true);
        m_animator->addAnimation("run", std::move(runAnim));
        
        Animation jumpAnim;
        jumpAnim.addFrame(spriteSheet->createFrame(8, 0.2f));
        jumpAnim.addFrame(spriteSheet->createFrame(9, 0.2f));
        jumpAnim.setLooping(false);
        m_animator->addAnimation("jump", std::move(jumpAnim));
    }

    m_animator->play("idle");
}

void Player::update(float deltaTime)
{
    Entity::update(deltaTime);

    position = m_physicsBody->position;

    if (!m_physicsBody->onGround)
    {
        m_animator->play("jump");
    }
    else if (std::abs(m_physicsBody->velocity.x) > 10.0f)
    {
        m_animator->play("run");
    }
    else
    {
        m_animator->play("idle");
    }

    handleInput(deltaTime);
}

void Player::handleInput(float deltaTime)
{
    float moveInput = 0.0f;
    if (InputManager::isKeyPressed(InputManager::getMappedKey("MoveLeft")))
    {
        moveInput -= 1.0f;
        m_facingRight = false;
    }
    if (InputManager::isKeyPressed(InputManager::getMappedKey("MoveRight")))
    {
        moveInput += 1.0f;
        m_facingRight = true;
    }

    m_physicsBody->velocity.x = moveInput * m_speed;

    if (InputManager::isKeyJustPressed(InputManager::getMappedKey("Jump")) && m_physicsBody->onGround)
    {
        m_physicsBody->velocity.y = -m_jumpForce;
    }

    if (InputManager::isKeyJustPressed(InputManager::getMappedKey("Dash")))
    {
        // implement Dash
    }
}
