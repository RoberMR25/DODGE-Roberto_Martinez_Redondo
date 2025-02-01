#pragma once

#include <SFML/Audio.hpp>
#include <string>

class AudioManager
{
    public:

        AudioManager();
        ~AudioManager();

        void playMusic(int state);    // state 1 = MainMenu | state 2 = InGame | state 3 = GameOver
        void stopMusic();
        void playSound(int soundIndex); // soundIndex 1 = Player Hurt | soundIndex 2 = coinPickUp
                                        // soundIndex 3 = heartPickUp | soundIndex 4 = energyPickUp
        void setVolume(float volume);

        bool menuMusicIsOn{ false };        // These bools will control, on Game.cpp, which
        bool inGameMusicIsOn{ false };      // music should be on depending on where we are
        bool gameOverMusicIsOn{ false };

    private:

        sf::Music m_music;
};