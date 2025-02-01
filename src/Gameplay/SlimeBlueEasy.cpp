#pragma once

#include <Gameplay/SlimeBlueEasy.h>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

bool SlimeBlueEasy::init(const SlimeBlueEasyDescriptor& slimeBlueEasyDescriptor)
{
	m_speed = slimeBlueEasyDescriptor.speed;
	m_direction = slimeBlueEasyDescriptor.direction;

	return Enemy::init(slimeBlueEasyDescriptor);
}

void SlimeBlueEasy::update(float deltaMilliseconds)
{
	m_position.x += m_direction.x * m_speed.x * (deltaMilliseconds / 1000.f);
	m_position.y += m_direction.y * m_speed.y * (deltaMilliseconds / 1000.f);

	Enemy::update(deltaMilliseconds);
}

sf::FloatRect SlimeBlueEasy::getBounds()	// We adjust the enemy bounds to our convenience for the game
{
	sf::FloatRect modifiedBounds = sf::FloatRect(
		m_sprite.getGlobalBounds().left + m_boundsOffsetX / 2,
		m_sprite.getGlobalBounds().top + m_boundsOffsetY / 2,
		m_sprite.getGlobalBounds().width - m_boundsOffsetX,
		m_sprite.getGlobalBounds().height - m_boundsOffsetY
	);

	return modifiedBounds;
}

void SlimeBlueEasy::render(sf::RenderWindow& window)
{
	m_color = sf::Color::Blue;
	Enemy::render(window);

	// Visual colliders
	/*
		const sf::FloatRect spriteBounds = getBounds();
		sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
		boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
		boundsRect.setOutlineColor(sf::Color::Magenta);
		boundsRect.setOutlineThickness(1.f);
		boundsRect.setFillColor(sf::Color::Transparent);
		window.draw(boundsRect);
	*/
}