#pragma once
#include "Window.h"
#include "Timer.h"
#include "../states/StateManager.h"
#include "../graphics/Camera.h"
#include <memory>

class Engine
{
public:
    Engine();
    virtual ~Engine();

    void run();


    virtual void onResize(int width, int height) = 0;

    Window& getWindow() { return *m_window; }
    StateManager& getStateManager() { return m_stateManager; }
    Camera& getCamera() { return *m_camera; }

    static Engine* getInstance() { return s_instance; }

protected:
    virtual void initialize() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

private:
    void mainLoop();

    std::unique_ptr<Window> m_window;
    std::unique_ptr<Camera> m_camera;
    StateManager m_stateManager;
    Timer m_timer;
    bool m_running = false;
    static Engine* s_instance;
};
