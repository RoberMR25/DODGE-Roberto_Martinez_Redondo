#pragma once

#include <Core/Game.h>
#include <iostream>
#include <SFML/Graphics.hpp>

class UIGameOverScreen
{
	public:

		UIGameOverScreen(float gameOverMenuWidth, float gameOverMenuHeight);
		void drawGameOverScreen(sf::RenderWindow& window);
		int mouseInput(sf::RenderWindow& window);

		sf::Text m_finalScore;
		sf::Text m_timeSurvived;
		sf::Text m_coinsCollected;

	private:

		sf::Texture m_gameOverTexture;
		sf::Sprite m_gameOverSprite;

		sf::Text m_gameOverButtons[3];	// Button 1 -> "Play again" | Button 2 -> "Return to menu" | Button 3 -> "Exit Game"

		sf::Text m_gameOverText;

		sf::Font m_font;
};