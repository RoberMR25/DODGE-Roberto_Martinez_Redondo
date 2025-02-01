#include <iostream>
#include <SFX/AudioManager.h>

AudioManager::AudioManager()
{
    m_music.setVolume(10.f);
}

AudioManager::~AudioManager()
{
    stopMusic();
}

void AudioManager::playMusic(int state)
{
    if (state == 1)
    {
        m_music.openFromFile("../Data/MusicAndSounds/MainMenuMusic.ogg");
        m_music.setLoop(true);
    }
    else if (state == 2)
    {
        m_music.openFromFile("../Data/MusicAndSounds/InGameMusic.ogg");
        m_music.setLoop(true);
    }
    else if (state == 3) 
    {
        m_music.openFromFile("../Data/MusicAndSounds/GameOverMusic.ogg");
        m_music.setLoop(false);
    }

    m_music.play();
}

void AudioManager::playSound(int soundIndex)
{
    if (soundIndex == 1)
        m_music.openFromFile("../Data/MusicAndSounds/PlayerHurtSound.wav");
    else if (soundIndex == 2)
        m_music.openFromFile("../Data/MusicAndSounds/CoinPickupSound.wav");
    else if (soundIndex == 3)
        m_music.openFromFile("../Data/MusicAndSounds/HealthPickupSound.wav");
    else if (soundIndex == 4)
        m_music.openFromFile("../Data/MusicAndSounds/EnergyPickupSound.wav");

    m_music.setLoop(false);
    m_music.play();
}

void AudioManager::stopMusic()
{
    m_music.stop();
}

void AudioManager::setVolume(float volume)
{
    m_music.setVolume(volume);
}