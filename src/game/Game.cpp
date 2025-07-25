#include "Game.h"
#include "../core/Logger.h"
#include "../core/ResourceManager.h"
#include "../graphics/Renderer.h"
#include "../physics/PhysicsEngine.h"
#include "../ui/UI.h"
#include "../audio/AudioManager.h"
#include "../core/InputManager.h"

void Game::initialize()
{
    Logger::info("Initializing game...");

    AudioManager::init();
    UIManager::init();
    PhysicsEngine::init();
    Renderer::init();

    m_editor = std::make_unique<LevelEditor>();

    getStateManager().pushState(std::make_unique<MenuState>());
}

void Game::onResize(int width, int height)
{

}

void Game::update(float deltaTime)
{
    if (InputManager::isKeyJustPressed(GLFW_KEY_E))
    {
        m_editor->setActive(!m_editor->isActive());
    }

    if (m_editor->isActive())
    {
        m_editor->update(deltaTime);
    }
    else
    {
        getStateManager().update(deltaTime); 
    }

    // auto stats = Renderer::getStats();
    // Logger::info("Draw calls: ", stats.drawCalls, " Quads count: ", stats.quadCount, " Index count: ", stats.indexCount, " Vertex count: ", stats.vertexCount);
}

void Game::render()
{
    if (m_editor->isActive())
    {
        m_editor->renderUI();
    }
    else
    {
        getStateManager().render();
    }
}

void Game::shutdown()
{
    LevelEditor::shutdown();
    ResourceManager::clear();
    Renderer::shutdown();
    Logger::info("Shutting down...");
}
