#pragma once

#include <Gameplay/PowerUps.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

bool PowerUps::init(const PowerUpsDescriptor& powerUpsDescriptor)
{
	m_sprite.setTexture(*powerUpsDescriptor.texture);
	m_sprite.setPosition(powerUpsDescriptor.position);
	m_tileWidth = powerUpsDescriptor.tileWidth;
	m_tileHeight = powerUpsDescriptor.tileHeight;

	return true;
}

void PowerUps::update(float deltaMilliseconds)
{
	if (m_isActive)
	{
		m_sprite.setPosition(m_position);

		m_time += deltaMilliseconds * m_millisecondsToSeconds;
		int seconds = static_cast<int>(m_time);
		if (seconds > m_previousSecond)
		{
			m_previousSecond = seconds;

			if (seconds % m_timeToDisappear == 0)
				setTimeExpired(true);
		}
	}
}

void PowerUps::render(sf::RenderWindow& window)
{
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_tileWidth, m_tileHeight));
	window.draw(m_sprite);

	// Visual collider
	/*
		const sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();
		sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
		boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
		boundsRect.setOutlineColor(sf::Color::Cyan);
		boundsRect.setOutlineThickness(1.f);
		boundsRect.setFillColor(sf::Color::Transparent);
		window.draw(boundsRect);
	*/
}

void PowerUps::resetTimers()
{
	m_time = 0.0f;
	m_previousSecond = 0;
	setTimeExpired(false);
}