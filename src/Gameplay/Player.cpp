#include <Gameplay/Player.h>

#include <iostream>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

bool Player::init(const PlayerDescriptor& playerDescriptor)
{
	m_speed = playerDescriptor.speed;
	m_defaultSpeed = playerDescriptor.defaultSpeed;
	m_sprintSpeed = playerDescriptor.sprintSpeed;

	m_tileWidth = playerDescriptor.tileWidth;
	m_tileHeight = playerDescriptor.tileHeight;

	if (playerDescriptor.texture == nullptr)
		return false;

	m_sprite.setTexture(*playerDescriptor.texture);

	m_sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(m_tileWidth), static_cast<int>(m_tileHeight)));

	m_sprite.setPosition(playerDescriptor.position);
	m_position = playerDescriptor.position;

	return true;
}

void Player::update(float deltaMilliseconds)
{
	m_time += deltaMilliseconds * m_millisecondsToSeconds;
	float deltaSeconds = deltaMilliseconds / 1000.0f;

	sf::Vector2f direction{ 0.0f, 0.0f };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_direction.x = -1.0f;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_direction.x = 1.0f;
	else
		m_direction.x = .0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_direction.y = 1.0f;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_direction.y = -1.0f;
	else
		m_direction.y = .0f;

	m_position.x += (m_direction.x * m_speed.x * deltaMilliseconds);
	m_position.y += (m_direction.y * m_speed.y * deltaMilliseconds);

	if (m_playerHealth > 0)
		playerSprint(deltaSeconds);

	m_sprite.setPosition(m_position);
}

void Player::render(sf::RenderWindow& window)
{
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_tileWidth, m_tileHeight));
	window.draw(m_sprite);

	// Visual collider
	/*
		const sf::FloatRect spriteBounds = getBounds();
		sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
		boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
		boundsRect.setOutlineColor(sf::Color::Yellow);
		boundsRect.setOutlineThickness(1.f);
		boundsRect.setFillColor(sf::Color::Transparent);
		window.draw(boundsRect);
	*/
}

void Player::loseHealth(int healthLost)
{
	if (m_playerHealth > 0)
	{
		m_playerHealth -= healthLost;
		if (m_playerHealth <= 0)
		{
			m_playerHealth = 0;
			setSpeed({ 0.f, 0.f });
		}
	}
}

void Player::playerSprint(float deltaSeconds)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) 
	{
		m_isSprinting = true;
		m_currentSprintEnergy -= (m_sprintEnergyLostPerSecond * deltaSeconds);

		if (m_currentSprintEnergy < 0.0f)
		{
			m_currentSprintEnergy = 0.0f;
			m_isSprinting = false;
		}
	}
	else
	{
		m_isSprinting = false;
		m_currentSprintEnergy += m_sprintEnergyRecoveredPerSecond * deltaSeconds;

		if (m_currentSprintEnergy > m_maxSprintEnergy)
			m_currentSprintEnergy = m_maxSprintEnergy;
	}

	if (m_isSprinting)
		setSpeed(m_sprintSpeed);
	else
		setSpeed(m_defaultSpeed);
}

void Player::recoverHealth(int healthRecovered)
{
	if (m_playerHealth < m_playerMaxHealth && m_playerHealth > 0)
		m_playerHealth += healthRecovered;
}

void Player::recoverEnergy(float energyRecovered)
{
	m_currentSprintEnergy += energyRecovered;
	if (m_currentSprintEnergy > m_maxSprintEnergy)
		m_currentSprintEnergy = m_maxSprintEnergy;
}