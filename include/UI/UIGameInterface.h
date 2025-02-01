#pragma once

#include <Core/Game.h>
#include <iostream>
#include <SFML/Graphics.hpp>

class UIGameInterface
{
	public:

		UIGameInterface(float gameInterfaceWidth, float gameInterfaceHeight);
		void drawGameInterface(sf::RenderWindow& window);

		sf::Text m_gameInterfaceScoreText;
		sf::Text m_gameInterfaceTimerText;
		sf::Text m_gameInterfaceCoinText;
		sf::Text m_gameInterfaceHealthText;
		sf::Text m_gameInterfaceEnergyText;

	private:

		sf::Font m_font;
};