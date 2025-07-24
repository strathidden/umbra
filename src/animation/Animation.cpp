#include "Animation.h"

void Animation::addFrame(const Frame& frame)
{
    m_frames.push_back(frame);
}

const Frame& Animation::getCurrentFrame() const
{
    return m_frames[m_currentFrame];
}

void Animation::update(float deltaTime)
{
    if (m_frames.empty()) return;
    if (m_finished) return;

    m_frameTimer += deltaTime;
    if (m_frameTimer >= m_frames[m_currentFrame].duration)
    {
        m_currentFrame++;
        m_frameTimer = 0.0f;

        if (m_currentFrame >= m_frames.size())
        {
            if (m_looping)
            {
                m_currentFrame = 0;
            }
            else
            {
                m_currentFrame = m_frames.size() - 1;
                m_finished = true;
            }
        }
    }
}

void Animation::reset()
{
    m_currentFrame = 0;
    m_frameTimer = 0.0f;
    m_finished = false;
}
