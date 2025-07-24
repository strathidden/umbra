#include "MenuState.h"
#include "PlayState.h"
#include "../core/Engine.h"

void MenuState::enter()
{
    m_startButton = std::make_unique<Button>("Start Game");
    m_startButton->position = {500.0f, 300.0f};
    m_startButton->size = {200.0f, 50.0f};
    m_startButton->setOnClick([this]() {
        Engine::getStateManager().changeState(std::make_unique<PlayState>());
    });

    m_quitButton = std::make_unique<Button>("Quit");
    m_quitButton->position = {500.0f, 400.0f};
    m_quitButton->size = {200.0f, 50.0f};
    m_quitButton->setOnClick([]() {
        Engine::getWindow().setShouldClose(true);
    });

    UIManager::addElement(m_startButton.get());
    UIManager::addElement(m_quitButton.get());
}

void MenuState::exit()
{
    UIManager::removeElement(m_startButton.get());
    UIManager::removeElement(m_quitButton.get());
}

void MenuState::update(float deltaTime)
{
    UIManager::update(deltaTime);
}

void MenuState::render()
{
    UIManager::render();
}
