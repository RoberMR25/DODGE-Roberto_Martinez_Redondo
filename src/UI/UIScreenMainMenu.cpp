#include <UI/UIScreenMainMenu.h>

UIScreenMainMenu::UIScreenMainMenu(float mainMenuWidth, float mainMenuHeight)
{
	// Load font
	m_font.loadFromFile("../Data/Fonts/SMB2.ttf");

	// Load background image
	m_mainMenuBackgroundTexture.loadFromFile("../Data/Images/UI/mainMenuBackgroundV3.png");
	m_mainMenuBackgroundSprite.setTexture(m_mainMenuBackgroundTexture);

	// Game title
	m_gameTitleText.setFont(m_font);
	m_gameTitleText.setString("DODGE!");
	m_gameTitleText.setCharacterSize(110);
	m_gameTitleText.setFillColor(sf::Color::Yellow);
	m_gameTitleText.setPosition(275, 50);

	// Game description
	m_gameDescriptionText.setFont(m_font);
	m_gameDescriptionText.setString("Dodge your enemies while collecting coins \n"
									"   to get the highest score possible. \n"
									"Recover lives and recharge your sprint to \n"
									"        try to survive longer. \n"
									"      What score can you achieve?");
	m_gameDescriptionText.setCharacterSize(13);
	m_gameDescriptionText.setFillColor(sf::Color::Yellow);
	m_gameDescriptionText.setPosition(325, 570);

	// Controls
	m_controlsText.setFont(m_font);
	m_controlsText.setString("MOVE:WASD   SPRINT:LSHIFT   PAUSE:P");
	m_controlsText.setCharacterSize(17);
	m_controlsText.setFillColor(sf::Color::Yellow);
	m_controlsText.setPosition(290, 715);

	std::string buttons[] = { "PLAY", "EXIT GAME" };
	for (int i = 0; i < 2; i++)
	{
		m_mainMenuButtons[i].setFont(m_font);
		m_mainMenuButtons[i].setString(buttons[i]);
		m_mainMenuButtons[i].setCharacterSize(40);
		m_mainMenuButtons[i].setFillColor(sf::Color::White);
		if (i==0)	// Play Button
			m_mainMenuButtons[i].setPosition(490, 280);
		else if (i==1)	// Exit Game Button
			m_mainMenuButtons[i].setPosition(390, 395);
	}
}

void UIScreenMainMenu::drawMainMenu(sf::RenderWindow& window)
{
	window.draw(m_mainMenuBackgroundSprite);
	window.draw(m_gameTitleText);
	window.draw(m_gameDescriptionText);
	window.draw(m_controlsText);

	for (int i = 0; i < 2; i++)
		window.draw(m_mainMenuButtons[i]);
}

int UIScreenMainMenu::mouseInput(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f pixelToCoordsMousePos = window.mapPixelToCoords(mousePos);	// It is recommended to use this if making customized view settings

	for (int i = 0; i < 2; i++)
	{
		// Because of the font, bounds do not work properly, so we make customized bounds for the buttons
		sf::FloatRect customBounds = m_mainMenuButtons[i].getGlobalBounds();

		customBounds.left -= 10.f;
		customBounds.width += 20.f;
		customBounds.top -= 10.f;
		customBounds.height += 20.f;

		if (customBounds.contains(pixelToCoordsMousePos))	// We use our customized bounds and pixelToCoordsMousePos
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				return i; // 1 -> Play     2 -> Controls     3 -> ExitGame
	}

	return -1; // When there's no click
}