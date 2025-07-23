#pragma once
#include <chrono>

class Timer
{
public:
    Timer();
    float tick();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTime;
};
