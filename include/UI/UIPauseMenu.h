#pragma once

#include <Core/Game.h>
#include <iostream>
#include <SFML/Graphics.hpp>

class UIPauseMenu
{
	public:

		UIPauseMenu(float pauseMenuWidth, float pauseMenuHeight);
		void drawPauseMenu(sf::RenderWindow& window);
		int mouseInput(sf::RenderWindow& window);

	private:

		sf::Texture m_pauseTexture;
		sf::Sprite m_pauseSprite;

		sf::Text m_pauseButtons[3];	// Button 1 -> "Play" | Button 2 -> "Return to menu" | Button 3 -> "Exit Game"

		sf::Font m_font;
};