#pragma once

#include <Core/Game.h>
#include <iostream>
#include <SFML/Graphics.hpp>

class UIScreenMainMenu
{
	public:

		UIScreenMainMenu(float mainMenuWidth, float mainMenuHeight);
		void drawMainMenu(sf::RenderWindow& window);
		int mouseInput(sf::RenderWindow& window);

	private:

		sf::Texture m_mainMenuBackgroundTexture;
		sf::Sprite m_mainMenuBackgroundSprite;

		sf::Text m_mainMenuButtons[2];	// 1 button for "Play", another one for "Exit Game"

		sf::Text m_gameTitleText;
		sf::Text m_gameDescriptionText;
		sf::Text m_controlsText;

		sf::Font m_font;
};