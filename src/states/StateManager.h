#pragma once
#include "GameState.h"
#include <stack>
#include <memory>

class StateManager
{
public:
    void pushState(std::unique_ptr<GameState> state);
    void popState();
    void changeState(std::unique_ptr<GameState> state);

    void update(float deltaTime);
    void render();

    bool isEmpty() const { return m_states.empty; }

private:
    std::stack<std::unique_ptr<GameState>> m_states;
};
