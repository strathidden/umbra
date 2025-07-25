#include "Engine.h"
#include "InputManager.h"

std::unique_ptr<Window> Engine::s_window;
StateManager Engine::s_stateManager;
Entity* Engine::s_player = nullptr;

Engine::Engine()
{
    s_window = std::make_unique<Window>(WindowProps{"Umbra", 1920, 1080, true});

    s_window->setResizeCallback([this](int width, int height) {
        onResize(width, height);
    });

    InputManager::init(s_window->getNativeWindow());
}

Engine::~Engine() = default;

void Engine::run()
{
    initialize();
    onResize(s_window->getWidth(), s_window->getHeight());
    m_running = true;
    mainLoop();
    shutdown();
}

void Engine::mainLoop()
{
    while (m_running && !glfwWindowShouldClose(s_window->getNativeWindow()))
    {
        float deltaTime = m_timer.tick();
        InputManager::update();
        update(deltaTime);
        render();
        s_window->onUpdate();
    }
}
