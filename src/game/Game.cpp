#include "Game.h"
#include "../core/Logger.h"
#include "../core/ResourceManager.h"
#include "../graphics/Renderer.h"
#include "../core/InputManager.h"
#include "../states/MenuState.h"
#include "../audio/AudioManager.h"

void Game::initialize()
{
    Logger::info("Initializing game...");

    m_levelEditor = std::make_unique<LevelEditor>();

    m_player = std::make_unique<Player>();

    getStateManager().pushState(std::make_unique<MenuState>());
}

void Game::onResize(int width, int height)
{

}

void Game::update(float deltaTime)
{
    if (InputManager::isKeyJustPressed(GLFW_KEY_F1))
    {
        m_editorActive = !m_editorActive;
        if (m_editorActive)
        {
            m_levelEditor->init(nullptr);
        }
    }

    if (m_editorActive)
    {
        m_levelEditor->update(deltaTime);
    }
    else
    {
        getStateManager().update(deltaTime);
    }

    PhysicsEngine::update(deltaTime);


    // auto stats = Renderer::getStats();
    // Logger::info("Draw calls: ", stats.drawCalls, " Quads count: ", stats.quadCount, " Index count: ", stats.indexCount, " Vertex count: ", stats.vertexCount);
}

void Game::render()
{
    if (m_editorActive)
    {
        m_levelEditor->renderUI();
    }
    else
    {
        getStateManager().render();
    }
}

void Game::shutdown()
{
    Logger::info("Shutting down...");
    PhysicsEngine::shutdown();
    AudioManager::shutdown();
    Renderer::shutdown();
    ResourceManager::clear();
}
