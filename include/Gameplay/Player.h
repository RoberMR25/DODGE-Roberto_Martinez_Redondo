#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Player : public GameObject
{
	public:

		Player() { m_sprite.setScale(1.5f, 1.5f); }

		struct PlayerDescriptor
		{
			sf::Vector2f position{ .0f, .0f };;
			sf::Vector2f speed{ .0f, .0f };
			sf::Vector2f defaultSpeed{ .0f, .0f };
			sf::Vector2f sprintSpeed{ .0f, .0f };

			sf::Texture* texture{ nullptr };
			float tileWidth{ 64.0f };
			float tileHeight{ 64.0f };
		};

		bool init(const PlayerDescriptor& playerDescriptor);

		sf::FloatRect getBounds() const
		{
			sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();

			spriteBounds.left += 37.5f;
			spriteBounds.top += 38.f;
			spriteBounds.width -= 73.f;
			spriteBounds.height -= 75.f;

			return spriteBounds;
		}

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

		void loseHealth(int healthLost);
		int getHealth() { return m_playerHealth; }

		int getSprintEnergy() { return m_currentSprintEnergy; }
		float getSprintEnergyWithDecimals() { return m_currentSprintEnergy; }

		void setSpeed(sf::Vector2f newSpeed) { m_speed = newSpeed; }

		void playerSprint(float deltaSeconds);

		void recoverHealth(int healthRecovered);
		void recoverEnergy(float energyRecovered);
		float energyRecoveredWhenPowerUpPicked{ 1.5f };

	private:

		float m_millisecondsToSeconds{ 1.f / 1000.f };
		float m_time{ 0.f };

		sf::Vector2f m_direction{ .0f, .0f };
		sf::Vector2f m_speed{ .0f, .0f };
		sf::Vector2f m_defaultSpeed{ 400.0f, 400.0f };
		sf::Vector2f m_sprintSpeed{ 800.0f, 800.0f };

		sf::Sprite m_sprite;
		float m_tileWidth{ 32.0f };
		float m_tileHeight{ 32.0f };

		int m_playerMaxHealth{ 3 };
		int m_playerHealth{ 3 };

		bool m_isSprinting{ false };
		float m_maxSprintEnergy{ 3.0f };
		float m_currentSprintEnergy{ 3.0f };
		float m_sprintEnergyLostPerSecond{ 1.0f };
		float m_sprintEnergyRecoveredPerSecond{ 0.5f };
};