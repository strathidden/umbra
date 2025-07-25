#pragma once
#include "Window.h"
#include "Timer.h"
#include "../states/StateManager.h"
#include "../entities/Entity.h"
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

    static Window& getWindow() { return *s_window; }
    static StateManager& getStateManager() { return s_stateManager; }
    static Entity* getPlayer() { return s_player; }

private:
    void mainLoop();

    static std::unique_ptr<Window> s_window;
    Timer m_timer;
    bool m_running = false;
    static StateManager s_stateManager;
    static Entity* s_player;
};
