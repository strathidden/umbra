#include "Engine.h"
#include "InputManager.h"
#include "../entities/Entity.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "../graphics/Renderer.h"
#include "../audio/AudioManager.h"
#include "../physics/PhysicsEngine.h"
#include "../ui/UI.h"

Engine* Engine::s_instance = nullptr;

Engine::Engine()
{
    s_instance = this;

    m_window = std::make_unique<Window>(WindowProps{"Umbra", 1920, 1080, true});

    m_window->setResizeCallback([this](int width, int height) {
        onResize(width, height);
    });
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
    m_camera = std::make_unique<Camera>();
    m_camera->setProjection(0.0f, 1920.0f, 1080.0f, 0.0f);

    InputManager::init(m_window->getNativeWindow());
    AudioManager::init();
    PhysicsEngine::init();
    Renderer::init();
    UIManager::init();

    while (m_running && !glfwWindowShouldClose(m_window->getNativeWindow()))
    {
        float deltaTime = m_timer.tick();

        InputManager::update();

        update(deltaTime);

        render();

        m_window->onUpdate();
    }
}
