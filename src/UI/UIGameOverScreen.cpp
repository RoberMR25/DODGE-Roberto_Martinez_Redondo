#include <UI/UIGameOverScreen.h>

UIGameOverScreen::UIGameOverScreen(float gameOverMenuWidth, float gameOverMenuHeight)
{
	// Load font:
	m_font.loadFromFile("../Data/Fonts/SMB2.ttf");

	m_gameOverTexture.loadFromFile("../Data/Images/UI/pauseBackground.png");
	m_gameOverSprite.setTexture(m_gameOverTexture);
	m_gameOverSprite.setColor(sf::Color(255, 255, 255, 50));

	// Game Over Text
	m_gameOverText.setFont(m_font);
	m_gameOverText.setString("GAME OVER");	// (Example to see how it fits)
	m_gameOverText.setCharacterSize(150);
	m_gameOverText.setFillColor(sf::Color::Red);
	m_gameOverText.setPosition(290, 100);

	// Final Score Text
	m_finalScore.setFont(m_font);
	m_finalScore.setString("Final score: 100");	// (Example to see how it fits)
	m_finalScore.setCharacterSize(60);
	m_finalScore.setFillColor(sf::Color::Magenta);
	m_finalScore.setPosition(510, 650);

	// Time survived Text
	m_timeSurvived.setFont(m_font);
	m_timeSurvived.setString("Time survived: 00:00:00");	// (Example to see how it fits)
	m_timeSurvived.setCharacterSize(60);
	m_timeSurvived.setFillColor(sf::Color::Cyan);
	m_timeSurvived.setPosition(270, 525);

	// Coins collected Text
	m_coinsCollected.setFont(m_font);
	m_coinsCollected.setString("Gems collected: 33");	// (Example to see how it fits)
	m_coinsCollected.setCharacterSize(60);
	m_coinsCollected.setFillColor(sf::Color::Yellow);
	m_coinsCollected.setPosition(440, 400);

	std::string buttons[] = { "PLAY AGAIN", "RETURN TO MENU", "EXIT GAME" };	// (Example to see how it fits)
	for (int i = 0; i < 3; i++)
	{
		m_gameOverButtons[i].setFont(m_font);
		m_gameOverButtons[i].setString(buttons[i]);
		m_gameOverButtons[i].setCharacterSize(40);
		m_gameOverButtons[i].setFillColor(sf::Color::White);

		if (i == 0)	// Play Again Button
			m_gameOverButtons[i].setPosition(150, 925);
		else if (i == 1)	// Return to Menu Button
			m_gameOverButtons[i].setPosition(700, 925);
		else if (i == 2)	// Exit Game Button
			m_gameOverButtons[i].setPosition(1410, 925);
	}
}

void UIGameOverScreen::drawGameOverScreen(sf::RenderWindow& window)
{
	window.draw(m_gameOverSprite);

	window.draw(m_gameOverText);
	window.draw(m_finalScore);
	window.draw(m_timeSurvived);
	window.draw(m_coinsCollected);

	for (int i = 0; i < 3; i++)
		window.draw(m_gameOverButtons[i]);
}

int UIGameOverScreen::mouseInput(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f pixelToCoordsMousePos = window.mapPixelToCoords(mousePos);	// It is recommended to use this if making customized view settings

	for (int i = 0; i < 3; i++)
	{
		// Because of the font, bounds do not work properly, so we make customized bounds for the buttons
		sf::FloatRect customBounds = m_gameOverButtons[i].getGlobalBounds();

		customBounds.left -= 10.f;
		customBounds.width += 20.f;
		customBounds.top -= 10.f;
		customBounds.height += 20.f;

		if (customBounds.contains(pixelToCoordsMousePos))	// We use our customized bounds and pixelToCoordsMousePos
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				return i; // 1 -> Play again     2 -> Return to menu     3 -> Exit Game
	}

	return -1; // When there's no click
}