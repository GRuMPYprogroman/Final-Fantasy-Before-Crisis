#include "AudioManager.h"
#include <iostream>
#include <algorithm>

bool AudioManager::init()
{
    try {
        loadAllSounds();
        loadAllMusic();
    }
    catch (const std::exception& e) {
        std::cerr << "AudioManager initialization failed: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void AudioManager::loadAllSounds()
{
    std::unordered_map<SoundID, std::string> soundFiles = {
        { SoundID::Choose, "../sounds/Menu Selection Click.wav" },
        { SoundID::Click,  "../sounds/Menu Selection Click.wav" },
		{SoundID::Buy, "../sounds/buy.ogg"},
        {SoundID::Inventory, "../sounds/inventory.wav"},
		{SoundID::Upgrade, "../sounds/upgrade.wav"},
        {SoundID::Flip, "../sounds/book_flip.ogg"},
        {SoundID::Pistol, "../sounds/pistol.wav"},
        {SoundID::Rifle, "../sounds/rifle.wav"},
        {SoundID::ColdWeapon, "../sounds/coldWeapon.ogg"},
        {SoundID::Punch, "../sounds/Punch.flac"},
        {SoundID::MaleScream, "../sounds/maleScream.ogg"},
        {SoundID::FemaleScream, "../sounds/female.ogg"},
    };

    for (auto& [id, path] : soundFiles) {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(path)) {
            throw std::runtime_error("Failed to load sound file: " + path);
        }
        _buffers.emplace(id, std::move(buffer));
    }
}

void AudioManager::loadAllMusic()
{
     _musicFiles = {
         { MusicID::MenuTheme,      "../music/mainMenuTheme.ogg" },
         { MusicID::GameplayTheme, "../music/GameplayTheme.ogg" },
         { MusicID::ExplorationTheme, "../music/explorationTheme.ogg" },
         { MusicID::BattleTheme, "../music/battleTheme.mp3" }
    };
}

sf::Sound* AudioManager::spawnSoundInstance(SoundID id)
{
    auto it = _buffers.find(id);
    if (it == _buffers.end()) {
        std::cerr << "Sound ID not found" << std::endl;
        return nullptr;
    }
    auto sound = std::make_unique<sf::Sound>(it->second);
    sf::Sound* ptr = sound.get();
    _activeSounds.push_back(std::move(sound));
    return ptr;
}

void AudioManager::playSound(SoundID id, float volume, float pitch)
{
    volume = std::clamp(volume, 0.f, 100.f);
    pitch = std::clamp(pitch, 0.f, 1.f);

    sf::Sound* sound = spawnSoundInstance(id);
    if (!sound) return;

    float finalVolume = std::clamp(volume * _masterVolume, 0.f, 100.f);
    sound->setVolume(finalVolume);
    sound->setPitch(pitch);
    sound->play();
}

void AudioManager::playMusic(MusicID id, float fadeInSec)
{
    auto it = _musicFiles.find(id);
    if (it == _musicFiles.end()) {
        std::cerr << "Music ID not found" << std::endl;
        return;
    }

    if (_currentMusic) {
        _currentMusic->stop();
        _currentMusic.reset();
    }

    _currentMusic = std::make_unique<sf::Music>();
    if (!_currentMusic->openFromFile(it->second)) {
        std::cerr << "Failed to load music file: " << it->second << std::endl;
        _currentMusic.reset();
        return;
    }
    _currentMusic->setLooping(true);

    _startVolume = 0.f;
    _targetVolume = _masterVolume * 100.f;
    _fadeDuration = fadeInSec;
    _fadeTimer = 0.f;
    _currentMusic->setVolume(0.f);
    _currentMusic->play();
    _fadeState = FadeState::FadingIn;
}

void AudioManager::stopMusic(float fadeOutSec)
{
    if (!_currentMusic) return;
    _startVolume = _currentMusic->getVolume();
    _targetVolume = 0.f;
    _fadeDuration = fadeOutSec;
    _fadeTimer = 0.f;
    _fadeState = FadeState::FadingOut;
}

void AudioManager::setMasterVolume(float vol)
{
    _masterVolume = std::clamp(vol, 0.f, 1.f);
    if (_currentMusic) {
        float musicVolume = std::clamp(_masterVolume * 100.f, 0.f, 100.f);
        _currentMusic->setVolume(musicVolume);
    }
}

void AudioManager::update(float dt)
{
    _activeSounds.erase(
        std::remove_if(_activeSounds.begin(), _activeSounds.end(),
            [](const std::unique_ptr<sf::Sound>& s) {
                return s->getStatus() == sf::Sound::Status::Stopped;
            }),
        _activeSounds.end()
    );

    if (_currentMusic && _fadeState != FadeState::Idle)
    {
        _fadeTimer += dt;
        float t = std::clamp(_fadeTimer / _fadeDuration, 0.f, 1.f);
        float vol = _startVolume + (_targetVolume - _startVolume) * t;
        _currentMusic->setVolume(vol);

        if (t >= 1.f)
        {
            if (_fadeState == FadeState::FadingOut)
            {
                _currentMusic->stop();
                _currentMusic.reset();
            }
            _fadeState = FadeState::Idle;
        }
    }
}