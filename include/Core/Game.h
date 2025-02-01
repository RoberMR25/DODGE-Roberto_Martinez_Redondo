#pragma once

#include <cstdint>
#include <SFML/Graphics.hpp>
#include <string>

namespace sf
{
	class RenderWindow;
}

class World;
class UIScreenMainMenu;
class UIPauseMenu;
class UIGameInterface;
class UIGameOverScreen;
class AudioManager;

class Game
{
	public:

		struct GameCreateInfo
		{
			std::string gameTitle;
			uint32_t screenWidth;
			uint32_t screenHeight;
			uint32_t frameRateLimit;
		};

		~Game();

		bool init(GameCreateInfo& createInfo);

		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();

		void updateGameInterface();
		void updateGameOverInterface();

		void updateTimer(uint32_t deltaMilliseconds);

		void resetGame();

	private:

		sf::RenderWindow* m_window{ nullptr };
		World* m_world{ nullptr };
		sf::View m_mainMenuView;
		sf::View m_gameView;
		sf::View m_pauseMenuView;
		sf::View m_gameInterfaceView;
		sf::View m_gameOverView;

		UIScreenMainMenu* m_mainMenu{ nullptr };
		UIPauseMenu* m_pauseMenu{ nullptr };
		UIGameInterface* m_gameInterface{ nullptr };
		UIGameOverScreen* m_gameOver{ nullptr };
		AudioManager* m_audioManager{ nullptr };

		enum GameState { MAINMENU, INGAME, PAUSE, GAMEOVER };
		GameState m_gameState{ MAINMENU };
		bool m_pauseKeyIsPressed{ false };


		float m_millisecondsToSeconds{ 0.f };
		int m_previousSecond{ 0 };
		float m_time{ 0.f };

		int m_hours{ 0 };
		int m_minutes{ 0 };
		int m_seconds{ 0 };
};