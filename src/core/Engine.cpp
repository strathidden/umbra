#include "Engine.h"
#include "InputManager.h"

Engine::Engine()
{
    m_window = std::make_unique<Window>(WindowProps{"Umbra", 1920, 1080, true});

    m_window->setResizeCallback([this](int width, int height) {
        onResize(width, height);
    });

    InputManager::init(m_window->getNativeWindow());
}

Engine::~Engine() = default;

void Engine::run()
{
    initialize();
    onResize(m_window->getWidth(), m_window->getHeight());
    m_running = true;
    mainLoop();
    shutdown();
}

void Engine::mainLoop()
{
    while (m_running && !glfwWindowShouldClose(m_window->getNativeWindow()))
    {
        float deltaTime = m_timer.tick();
        InputManager::update();
        update(deltaTime);
        render();
        m_window->onUpdate();
    }
}
