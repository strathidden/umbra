#pragma once
#include "Window.h"
#include "Timer.h"
#include "../states/StateManager.h"
#include "../entities/Enemy.h"
#include <memory>

class Engine
{
public:
    Engine();
    virtual ~Engine();

    void run();

    virtual void initialize() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    virtual void onResize(int width, int height) = 0;

    Window& getWindow() { return *m_window; }
    StateManager& getStateManager() { return m_stateManager; }
    static Entity* getPlayer() { return s_player; }

private:
    void mainLoop();

    std::unique_ptr<Window> m_window;
    Timer m_timer;
    bool m_running = false;
    StateManager m_stateManager;
    static Entity* s_player;
};
