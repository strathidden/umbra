#include "Sound.h"
#include "AudioManager.h"

Sound::~Sound()
{
    if (m_source)
    {
        AudioManager::deleteSource(m_source);
    }
}

bool Sound::load(const std::string& filepath)
{
    m_buffer = AudioManager::loadSound(filepath);
    if (!m_buffer)
    {
        return false;
    }

    m_source = AudioManager::createSource();
    alSourcei(m_source, AL_BUFFER, m_buffer);
    return true;
}

void Sound::play()
{
    if (m_source)
    {
        AudioManager::playSound(m_source);
    }
}

void Sound::stop()
{
    if (m_source)
    {
        AudioManager::stopSound(m_source);
    }
}

void Sound::setLooping(bool looping)
{
    if (m_source)
    {
        alSourcei(m_source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
    }
}
