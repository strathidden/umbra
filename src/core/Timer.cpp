#include "Timer.h"

Timer::Timer() : m_lastTime(std::chrono::high_resolution_clock::now()) {}

float Timer::tick()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - m_lastTime);
    m_lastTime = currentTime;
    return duration.count() / 1000000.0f;
}
