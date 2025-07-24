#include "AudioManager.h"
#include "../core/Logger.h"
#include <stb_vorbis.c>

ALCdevice* AudioManager::s_device = nullptr;
ALCcontext* AudioManager::s_context = nullptr;
std::unordered_map<std::string, ALuint> AudioManager::s_buffers;

bool AudioManager::init()
{
    s_device = alcOpenDevice(nullptr);
    if (!s_device)
    {
        Logger::error("Failed to open OpenAL device");
        return false;
    }

    s_context = alcCreateContext(s_device, nullptr);
    if (!s_context || !alcMakeContextCurrent(s_context))
    {
        Logger::error("Failed to create OpenAL context");
        return false;
    }

    Logger::info("OpenAL initialized");
    return true;
}

void AudioManager::shutdown()
{
    for (auto& buffer : s_buffers)
    {
        alDeleteBuffers(1, &buffer.second);
    }
    s_buffers.clear();

    alcMakeContextCurrent(nullptr);
    if (s_context)
    {
        alcDestroyContext(s_context);
    }
    if (s_device)
    {
        alcCloseDevice(s_device);
    }
}

ALuint AudioManager::loadSound(const std::string& filepath)
{
    auto it = s_buffers.find(filepath);
    if (it != s_buffers.end())
    {
        return it->second;
    }

    ALuint buffer = 0;
    int channels;
    int sampleRate;
    short* data;
    int samples = stb_vorbis_decode_filename(filepath.c_str(), &channels, &sampleRate, &data);
    if (samples == -1)
    {
        Logger::error("Failed to load sound: ", filepath);
        return 0;
    }

    ALenum format;
    if (channels == 1)
    {
        format = AL_FORMAT_MONO16;
    }
    else if (channels == 2)
    {
        format = AL_FORMAT_STEREO16;
    }
    else
    {
        Logger::error("Unsupported audio format: ", filepath);
        free(data);
        return 0;
    }

    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, data, samples * channels * sizeof(short), sampleRate);
    free(data);

    s_buffers[filepath] = buffer;
    return buffer;
}

ALuint AudioManager::createSource()
{
    ALuint source;
    alGenSources(1, &source);
    return source;
}

void AudioManager::deleteSource(ALuint source)
{
    alDeleteSources(1, &source);
}

void AudioManager::playSound(ALuint source)
{
    alSourcePlay(source);
}

void AudioManager::stopSound(ALuint source)
{
    alSourceStop(source);
}
