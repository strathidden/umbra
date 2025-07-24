#include "PlayState.h"
#include "PauseState.h"
#include "../core/Engine.h"
#include "../core/InputManager.h"

void PlayState::enter()
{
    m_tileMap.loadFromFile("assets/level1.csv");
    m_player = std::make_unique<Player>();
    // setup level
}

void PlayState::exit()
{
    // cleanup
}

void PlayState::update(float deltaTime)
{
    if (InputManager::isKeyJustPressed(InputManager::getMappedKey("Pause")))
    {
        Engine::getStateManager().pushState(std::make_unique<PauseState>());
    }

    m_player->update(deltaTime);
    PhysicsEngine::update(deltaTime);
}

void PlayState::render()
{
    m_tileMap.render();
    m_player->render();
}
