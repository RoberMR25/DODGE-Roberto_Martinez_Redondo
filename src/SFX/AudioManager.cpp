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

void AudioManager::playMusic(int state)    // state 1 = MainMenu | state 2 = InGame | state 3 = GameOver
{
    if (state == 1) // Main Menu
    {
        m_music.openFromFile("../Data/MusicAndSounds/MainMenuMusic.ogg");
        m_music.setLoop(true);
    }
    else if (state == 2) // InGame
    {
        m_music.openFromFile("../Data/MusicAndSounds/InGameMusic.ogg");
        m_music.setLoop(true);
    }
    else if (state == 3) // GameOver
    {
        m_music.openFromFile("../Data/MusicAndSounds/GameOverMusic.ogg");
        m_music.setLoop(false);     // In this case, we just want a short, characteristic song for the defeat,
    }                               // we don't want to have it looping

    m_music.play();
}

void AudioManager::playSound(int soundIndex)    // soundIndex 1 = Player Hurt | soundIndex 2 = coinPickUp
{                                               // soundIndex 3 = heartPickUp | soundIndex 4 = energyPickUp
    if (soundIndex == 1)        // Player Hurt
        m_music.openFromFile("../Data/MusicAndSounds/PlayerHurtSound.wav");
    else if (soundIndex == 2)   // coinPickUp
        m_music.openFromFile("../Data/MusicAndSounds/CoinPickupSound.wav");
    else if (soundIndex == 3)   // heartPickUp
        m_music.openFromFile("../Data/MusicAndSounds/HealthPickupSound.wav");
    else if (soundIndex == 4)   // energyPickUp
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