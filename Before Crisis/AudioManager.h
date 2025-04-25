#pragma once
#include "SFML/Audio.hpp"
#include "AudioVariables.h"
#include "AudioService.h"

class AudioManager : public AudioService
{
private:
	std::unordered_map<SoundID, sf::SoundBuffer> _buffers;
	std::unordered_map<MusicID, std::string> _musicFiles;
	std::vector<std::unique_ptr<sf::Sound>> _activeSounds;
	std::unique_ptr<sf::Music> _currentMusic;
	float _masterVolume = 1.f;
	void loadAllSounds();
	void loadAllMusic();
	sf::Sound* spawnSoundInstance(SoundID id);

	float _fadeDuration = 0.f;   // длительность текущего перехода
	float _fadeTimer = 0.f;   // сколько времени уже прошло
	float _startVolume = 0.f;   // громкость в начале перехода
	float _targetVolume = 0.f;   // к какой громкости хотим прийти
	enum class FadeState { Idle, FadingIn, FadingOut } _fadeState = FadeState::Idle;
public:
	bool init();
	void playSound(SoundID id, float volume = 60.f, float pitch = 1.f) override;
	void playMusic(MusicID id, float fadeInSec = 0.5f) override;
	void stopMusic(float fadeOutSec = 0.5f) override;
	void setMasterVolume(float vol) override;
	void update(float dt) override;
};

