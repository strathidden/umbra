#include "Game.h"

Game::Game()
{
    m_window = std::make_unique<Window>(WindowProps{"Umbra", 1920, 1080, true});
}

Game::~Game() = default;

void Game::run()
{
    initialize();
    m_running = true;
    mainLoop();
    shutdown();
}

void Game::mainLoop()
{
    while (m_running && !glfwWindowShouldClose(m_window->getNativeWindow()))
    {
        float deltaTime = m_timer.tick();
        update(deltaTime);
        render();
        m_window->onUpdate();
    }
}
