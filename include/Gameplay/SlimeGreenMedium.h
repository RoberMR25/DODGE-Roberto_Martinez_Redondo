#pragma once

#include <Gameplay/Enemy.h>

class SlimeGreenMedium : public Enemy
{
	public:

		SlimeGreenMedium()
		{
			m_sprite.setScale(0.8f, 0.8f);
			m_damage = 2;
		}

		struct SlimeGreenMediumDescriptor : EnemyDescriptor
		{
			sf::Vector2f speed{ .0f, .0f };
		};

		bool init(const SlimeGreenMediumDescriptor& enemyDescriptor);

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

		sf::FloatRect getBounds();

	private:

		sf::Vector2f m_direction{ .0f, .0f };
		sf::Vector2f m_speed{ 50.0f, 50.0f };

		float m_boundsOffsetX{ 9.f };
		float m_boundsOffsetY{ 9.f };
};