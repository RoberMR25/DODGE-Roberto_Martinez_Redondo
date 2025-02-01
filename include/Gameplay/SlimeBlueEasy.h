#pragma once

#include <Gameplay/Enemy.h>

class SlimeBlueEasy : public Enemy
{
	public:

		SlimeBlueEasy()
		{
			m_sprite.setScale(0.5f, 0.5f);
			m_damage = 1;
		}

		struct SlimeBlueEasyDescriptor : EnemyDescriptor
		{
			sf::Vector2f speed{ .0f, .0f };
		};

		bool init(const SlimeBlueEasyDescriptor& enemyDescriptor);

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

		sf::FloatRect getBounds();

	private:

		sf::Vector2f m_direction{ .0f, .0f };
		sf::Vector2f m_speed{ 100.0f, 100.0f };

		float m_boundsOffsetX{ 5.f };
		float m_boundsOffsetY{ 5.f };
};