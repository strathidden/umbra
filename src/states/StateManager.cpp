#include "StateManager.h"

void StateManager::pushState(std::unique_ptr<GameState> state)
{
    if (!m_states.empty())
    {
        m_states.top()->exit();
    }
    m_states.push(std::move(state));
    m_states.top()->enter();
}

void StateManager::popState()
{
    if (!m_states.empty())
    {
        m_states.top()->exit();
        m_states.pop();
    }
    if (!m_states.empty())
    {
        m_states.top()->enter();
    }
}

void StateManager::changeState(std::unique_ptr<GameState> state)
{
    while (!m_states.empty())
    {
        m_states.top()->exit();
        m_states.pop();
    }
    pushState(std::move(state));
}

void StateManager::update(float deltaTime)
{
    if (!m_states.empty())
    {
        m_states.top()->update(deltaTime);
    }
}

void StateManager::render()
{
    if (!m_states.empty())
    {
        m_states.top()->render();
    }
}
