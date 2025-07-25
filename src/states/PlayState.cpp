#include "PlayState.h"
#include "PauseState.h"
#include "../core/Engine.h"
#include "../core/InputManager.h"
#include "../graphics/Renderer.h"
#include "../core/Engine.h"
#include "../game/Game.h"
#include "../core/Logger.h"

void PlayState::enter()
{
    auto engine = Engine::getInstance();
    auto& game = static_cast<Game&>(*engine);

    m_tileMap.loadFromFile("assets/levels/test_area.umbra");

    game.getPlayer()->position = {200, 500};

    game.getLevelEditor().init(&m_tileMap);
}

void PlayState::exit()
{
    // cleanup
}

void PlayState::update(float deltaTime)
{
    auto engine = Engine::getInstance();
    auto& game = static_cast<Game&>(*engine);

    game.getPlayer()->update(deltaTime);

    glm::vec2 targetPos = game.getPlayer()->position - glm::vec2(400, 300);
    glm::vec2 cameraPos = engine->getCamera().getPosition();
    glm::vec2 newPos = cameraPos + (targetPos - cameraPos) * 0.1f;
    engine->getCamera().setPosition(newPos);

    if (InputManager::isKeyJustPressed(InputManager::getMappedKey("Pause")))
    {
        engine->getStateManager().pushState(std::make_unique<PauseState>());
    }
}

void PlayState::render()
{
    auto engine = Engine::getInstance();
    auto& game = static_cast<Game&>(*engine);

    Renderer::drawQuad({0, 0}, {1600, 900}, {0.1f, 0.1f, 0.2f, 1.0f});
    m_tileMap.render();
    game.getPlayer()->render();
}
