#include <UI/UIPauseMenu.h>

UIPauseMenu::UIPauseMenu(float pauseMenuWidth, float pauseMenuHeight)
{
	m_font.loadFromFile("../Data/Fonts/SMB2.ttf");

	m_pauseTexture.loadFromFile("../Data/Images/UI/pauseBackground.png");
	m_pauseSprite.setTexture(m_pauseTexture);
	m_pauseSprite.setColor(sf::Color(255, 255, 255, 50));

	std::string buttons[] = { "RESUME", "RETURN TO MENU", "EXIT GAME" };
	for (int i = 0; i < 3; i++)
	{
		m_pauseButtons[i].setFont(m_font);
		m_pauseButtons[i].setString(buttons[i]);
		m_pauseButtons[i].setCharacterSize(80);
		m_pauseButtons[i].setFillColor(sf::Color::Red);
		if (i == 0)	
			m_pauseButtons[i].setPosition(700, 300);
		else if (i == 1)
			m_pauseButtons[i].setPosition(400, 500);
		else if (i == 2)
			m_pauseButtons[i].setPosition(600, 700);
	}
}

void UIPauseMenu::drawPauseMenu(sf::RenderWindow& window)
{
	window.draw(m_pauseSprite);

	for (int i = 0; i < 3; i++)
		window.draw(m_pauseButtons[i]);
}

int UIPauseMenu::mouseInput(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f pixelToCoordsMousePos = window.mapPixelToCoords(mousePos);	// It is recommended to use this if making customized view settings

	for (int i = 0; i < 3; i++)
	{
		// Because of the font, bounds do not work properly, so we make customized bounds for the buttons
		sf::FloatRect customBounds = m_pauseButtons[i].getGlobalBounds();

		customBounds.left -= 10.f;
		customBounds.width += 20.f;
		customBounds.top -= 10.f;
		customBounds.height += 20.f;

		if (customBounds.contains(pixelToCoordsMousePos))
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				return i;
	}

	return -1;
}