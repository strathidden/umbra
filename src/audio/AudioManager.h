#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <unordered_map>

class AudioManager
{
public:
    static bool init();
    static void shutdown();

    static ALuint loadSound(const std::string& filepath);
    static void playSound(ALuint source);
    static void stopSound(ALuint source);

    static ALuint createSource();
    static void deleteSource(ALuint source);

private:
    static ALCdevice* s_device;
    static ALCcontext* s_context;
    static std::unordered_map<std::string, ALuint> s_buffers;
};

