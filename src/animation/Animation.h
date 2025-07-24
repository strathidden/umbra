#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Frame
{
    glm::vec2 texCoords[4];
    float duration;
};

class Animation
{
public:
    Animation() = default;
    void addFrame(const Frame& frame);
    const Frame& getCurrentFrame() const;
    void update(float deltaTime);
    void reset();
    void setLooping(bool looping) { m_looping = looping; }
    bool isFinished() const { return m_finished; }

private:
    std::vector<Frame> m_frames;
    size_t m_currentFrame = 0;
    float m_frameTimer = 0.0f;
    bool m_looping = true;
    bool m_finished = false;
};
