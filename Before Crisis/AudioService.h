#pragma once
#include "AudioVariables.h"

class AudioService {
public:
    virtual ~AudioService() = default;
    virtual void playSound(SoundID id, float volume = 100.f, float pitch = 1.f) = 0;
    virtual void playMusic(MusicID id, float fadeInSec = 2.f) = 0;
    virtual void stopMusic(float fadeOutSec = 2.f) = 0;
    virtual void setMasterVolume(float vol) = 0;
    virtual void update(float dt) = 0;
};