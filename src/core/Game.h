#pragma once
#include "Window.h"
#include "Timer.h"
#include <memory>

class Game
{
public:
    Game();
    virtual ~Game();

    void run();

    virtual void initialize() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    Window& getWindow() { return *m_window; }

protected:
    std::unique_ptr<Window> m_window;

private:
    void mainLoop();
    Timer m_timer;
    bool m_running = false;
};
