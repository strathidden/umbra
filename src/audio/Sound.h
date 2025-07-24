#pragma once
#include <AL/al.h>
#include <string>

class Sound
{
public:
    Sound() = default;
    ~Sound();

    bool load(const std::string& filepath);
    void play();
    void stop();
    void setLooping(bool looping);

private:
    ALuint m_source = 0;
    ALuint m_buffer = 0;
};
