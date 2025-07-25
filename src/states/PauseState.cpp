#include "PauseState.h"
#include "MenuState.h"
#include "../core/Engine.h"
#include "../graphics/Renderer.h"

void PauseState::enter()
{
    m_resumeButton = std::make_unique<Button>("Resume");
    m_resumeButton->position = {500.0f, 300.0f};
    m_resumeButton->size = {200.0f, 50.0f};
    m_resumeButton->setOnClick([this]() {
        Engine::getInstance()->getStateManager().popState();
    });

    m_menuButton = std::make_unique<Button>("Main Menu");
    m_menuButton->position = {500.0f, 400.0f};
    m_menuButton->size = {200.0f, 50.0f};
    m_menuButton->setOnClick([this]() {
        Engine::getInstance()->getStateManager().changeState(std::make_unique<MenuState>());
    });

    UIManager::addElement(m_resumeButton.get());
    UIManager::addElement(m_menuButton.get());
}

void PauseState::exit()
{
    UIManager::removeElement(m_resumeButton.get());
    UIManager::removeElement(m_menuButton.get());
}

void PauseState::update(float deltaTime)
{
    UIManager::update(deltaTime);
}

void PauseState::render()
{
    Renderer::drawQuad({0,0}, {1920,1080}, {0,0,0,0.5f});
    UIManager::render();
}
