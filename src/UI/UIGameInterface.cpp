#include <UI/UIGameInterface.h>

UIGameInterface::UIGameInterface(float gameInterfaceWidth, float gameInterfaceHeight)
{
	// Load font
	m_font.loadFromFile("../Data/Fonts/SMB2.ttf");

	// Score
	m_gameInterfaceScoreText.setFont(m_font);
	m_gameInterfaceScoreText.setString("SCORE: 1524");	// (Example to see how it fits)
	m_gameInterfaceScoreText.setCharacterSize(50);
	m_gameInterfaceScoreText.setFillColor(sf::Color::White);
	m_gameInterfaceScoreText.setPosition(315, 305);
	m_gameInterfaceScoreText.setScale(.1f, .1f);

	// Timer
	m_gameInterfaceTimerText.setFont(m_font);
	m_gameInterfaceTimerText.setString("Time: 00:00:00");	// (Example to see how it fits)
	m_gameInterfaceTimerText.setCharacterSize(50);
	m_gameInterfaceTimerText.setFillColor(sf::Color::White);
	m_gameInterfaceTimerText.setPosition(400, 305);
	m_gameInterfaceTimerText.setScale(.1f, .1f);

	// Coins
	m_gameInterfaceCoinText.setFont(m_font);
	m_gameInterfaceCoinText.setString("Coins: 137");	// (Example to see how it fits)
	m_gameInterfaceCoinText.setCharacterSize(50);
	m_gameInterfaceCoinText.setFillColor(sf::Color::White);
	m_gameInterfaceCoinText.setPosition(230, 305);
	m_gameInterfaceCoinText.setScale(.1f, .1f);

	// Health
	m_gameInterfaceHealthText.setFont(m_font);
	m_gameInterfaceHealthText.setString("Health: 3");	// (Example to see how it fits)
	m_gameInterfaceHealthText.setCharacterSize(50);
	m_gameInterfaceHealthText.setFillColor(sf::Color::White);
	m_gameInterfaceHealthText.setPosition(40, 305);
	m_gameInterfaceHealthText.setScale(.1f, .1f);

	// Energy
	m_gameInterfaceEnergyText.setFont(m_font);
	m_gameInterfaceEnergyText.setString("Energy: 3");	// (Example to see how it fits)
	m_gameInterfaceEnergyText.setCharacterSize(50);
	m_gameInterfaceEnergyText.setFillColor(sf::Color::White);
	m_gameInterfaceEnergyText.setPosition(125, 305);
	m_gameInterfaceEnergyText.setScale(.1f, .1f);
}

void UIGameInterface::drawGameInterface(sf::RenderWindow& window)
{
	window.draw(m_gameInterfaceScoreText);
	window.draw(m_gameInterfaceTimerText);
	window.draw(m_gameInterfaceCoinText);
	window.draw(m_gameInterfaceHealthText);
	window.draw(m_gameInterfaceEnergyText);
}