#include <cassert>
#include <iomanip>
#include <sstream>

#include <Core/Game.h>
#include <Core/World.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <SFX/AudioManager.h>

#include <UI/UIGameInterface.h>
#include <UI/UIGameOverScreen.h>
#include <UI/UIPauseMenu.h>
#include <UI/UIScreenMainMenu.h>

bool Game::init(GameCreateInfo& createInfo)
{
	assert(m_window == nullptr && m_world == nullptr && "Game is already initialized, we are about to leak memory");

	m_window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle);
	m_window->setFramerateLimit(createInfo.frameRateLimit);

	m_millisecondsToSeconds = 1 / 1000.f;
	m_time = 0.f;
	m_previousSecond = 0;
	m_seconds = 0.f;
	m_minutes = 0.f;
	m_hours = 0.f;

	m_mainMenuView = sf::View(sf::FloatRect({ 0.f, 0.f }, { 1144.f, 744.f }));	// This is the view we will have when we initiate the game
	m_mainMenuView.setCenter({ 577.f, 372.f });									// It is 1144x744 because that's the background png definition

	m_gameView = sf::View(sf::FloatRect({ 0.f, 0.f }, { 480.f, 320.f }));	// This is the game definition. Since the pixelart scenery and characters
	m_gameView.setCenter({ 240.f, 160.f });									// are so small, we "make a big zoom" to see them bigger

	m_pauseMenuView = sf::View(sf::FloatRect({ 0.f, 0.f }, { 1920.f, 1080.f }));
	m_pauseMenuView.setCenter({ 960.f, 540.f });

	m_gameOverView = sf::View(sf::FloatRect({ 0.f, 0.f }, { 1920.f, 1080.f }));
	m_gameOverView.setCenter({ 960.f, 540.f });

	m_window->setView(m_mainMenuView);

	m_mainMenu = new UIScreenMainMenu(1920.f, 1080.f);
	m_pauseMenu = new UIPauseMenu(1920.f, 1080.f);
	m_gameOver = new UIGameOverScreen(1920.f, 1080.f);
	m_gameInterface = new UIGameInterface(1920.f, 1080.f);
	m_audioManager = new AudioManager();
	m_audioManager->setVolume(7.f);

	m_world = new World();

	return true;
}

Game::~Game()
{
	delete m_audioManager;
	delete m_mainMenu;
	delete m_pauseMenu;
	delete m_gameOver;
	delete m_gameInterface;
	delete m_world;
	delete m_window;
}

bool Game::isRunning() const 
{ 
	return m_window->isOpen(); 
}

void Game::update(uint32_t deltaMilliseconds)
{
	for (auto event = sf::Event(); m_window->pollEvent(event);)
		if (event.type == sf::Event::Closed)
			m_window->close();

	if (m_gameState == INGAME && sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !m_pauseKeyIsPressed)
	{
		m_pauseKeyIsPressed = true;
		m_gameState = PAUSE;
	}
	else if (m_gameState == PAUSE && sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !m_pauseKeyIsPressed)
	{
		m_pauseKeyIsPressed = true;
		m_gameState = INGAME;
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		m_pauseKeyIsPressed = false;

	if (m_gameState == MAINMENU)
	{
		m_window->setView(m_mainMenuView);
		
		if (!m_audioManager->menuMusicIsOn)
		{
			m_audioManager->inGameMusicIsOn = false;
			m_audioManager->gameOverMusicIsOn = false;
			m_audioManager->menuMusicIsOn = true;

			m_audioManager->playMusic(1);
		}

		if (m_mainMenu->mouseInput(*m_window) == 0)
		{
			m_gameState = INGAME;
			resetGame();
		}
		else if (m_mainMenu->mouseInput(*m_window) == 1)
			m_window->close();
	}
	else if (m_gameState == INGAME)
	{
		m_window->setView(m_gameView);

		if (!m_audioManager->inGameMusicIsOn)
		{
			m_audioManager->menuMusicIsOn = false;
			m_audioManager->gameOverMusicIsOn = false;
			m_audioManager->inGameMusicIsOn = true;

			m_audioManager->playMusic(2);
		}

		m_world->update(deltaMilliseconds);
		updateTimer(deltaMilliseconds);
		updateGameInterface();
		if (m_world->getPlayerHealth() == 0)
			m_gameState = GAMEOVER;
	}
	else if (m_gameState == PAUSE)
	{
		m_window->setView(m_pauseMenuView);

		if (m_pauseMenu->mouseInput(*m_window) == 0)
			m_gameState = INGAME;
		else if (m_pauseMenu->mouseInput(*m_window) == 1)
		{
			m_gameState = MAINMENU;
			m_window->setView(m_mainMenuView);
		}
		else if (m_pauseMenu->mouseInput(*m_window) == 2)
			m_window->close();
	}
	else if (m_gameState == GAMEOVER)
	{
		m_window->setView(m_gameOverView);

		if (!m_audioManager->gameOverMusicIsOn)
		{
			m_audioManager->menuMusicIsOn = false;
			m_audioManager->inGameMusicIsOn = false;
			m_audioManager->gameOverMusicIsOn = true;

			m_audioManager->playMusic(3);
		}

		if (m_gameOver->mouseInput(*m_window) == 0)
		{
			m_gameState = INGAME;
			resetGame();
		}
		else if (m_gameOver->mouseInput(*m_window) == 1)
		{
			m_gameState = MAINMENU;
			m_window->setView(m_mainMenuView);
		}
		else if (m_gameOver->mouseInput(*m_window) == 2)
			m_window->close();
	}
}

void Game::render()
{
	m_window->clear();

	if (m_gameState == MAINMENU)
		m_mainMenu->drawMainMenu(*m_window);
	else if (m_gameState == PAUSE)
		m_pauseMenu->drawPauseMenu(*m_window);
	else if (m_gameState == INGAME)
	{
		m_world->render(*m_window);
		m_gameInterface->drawGameInterface(*m_window);
	}
	else if (m_gameState == GAMEOVER)
	{
		updateGameOverInterface();
		m_gameOver->drawGameOverScreen(*m_window);
	}

	m_window->display();
}


void Game::updateGameInterface()
{
	m_gameInterface->m_gameInterfaceScoreText.setString("Score: " + std::to_string(m_world->getScore()));
	
	m_gameInterface->m_gameInterfaceTimerText.setString(
		(m_hours < 10 ? "0" : "") + std::to_string(m_hours) + ":" +		// This way, if hours, minutes or seconds are 
		(m_minutes < 10 ? "0" : "") + std::to_string(m_minutes) + ":" + // under 10, a "0" appears before them, so that
		(m_seconds < 10 ? "0" : "") + std::to_string(m_seconds)			// the timer has always this format: "00:00:00"
	);

	m_gameInterface->m_gameInterfaceCoinText.setString("Gems: " + std::to_string(m_world->getCoinsCollected()));

	m_gameInterface->m_gameInterfaceHealthText.setString("Health: " + std::to_string(m_world->getPlayerHealth()));

	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2) << m_world->getPlayerEnergy();
	m_gameInterface->m_gameInterfaceEnergyText.setString("Energy: " + oss.str());
}

void Game::updateGameOverInterface()
{
	m_gameOver->m_coinsCollected.setString("Gems collected: " + std::to_string(m_world->getCoinsCollected()));

	m_gameOver->m_timeSurvived.setString(
		std::string("Time survived: ") +
		(m_hours < 10 ? "0" : "") + std::to_string(m_hours) + ":" +
		(m_minutes < 10 ? "0" : "") + std::to_string(m_minutes) + ":" +
		(m_seconds < 10 ? "0" : "") + std::to_string(m_seconds)
	);

	m_gameOver->m_finalScore.setString("Final score: " + std::to_string(m_world->getScore()));
}

void Game::updateTimer(uint32_t deltaMillisecondsForTimer)
{
	m_time += deltaMillisecondsForTimer * m_millisecondsToSeconds;

	int seconds = static_cast<int>(m_time);
	if (seconds > m_previousSecond)
	{
		m_previousSecond = seconds;
		m_seconds = seconds;

		if (m_seconds == 60)
		{
			++m_minutes;
			m_previousSecond = 0;
			m_time = 0.f;
			m_seconds = 0;
		}
		if (m_minutes == 60 && m_seconds < seconds)
		{
			++m_hours;
			m_minutes = 0;
		}
	}
}

void Game::resetGame()
{
	m_time = 0.f;
	m_previousSecond = 0;
	m_seconds = 0.f;
	m_minutes = 0.f;
	m_hours = 0.f;

	delete m_world;
	m_world = new World();
	m_world->load();
}