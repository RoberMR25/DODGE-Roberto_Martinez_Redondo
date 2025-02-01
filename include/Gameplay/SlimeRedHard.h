#pragma once

#include <Gameplay/Enemy.h>


class SlimeRedHard : public Enemy
{
public:
	
	SlimeRedHard()
	{
		m_sprite.setScale(1.0f, 1.0f);
		m_damage = 3;
	}

	struct SlimeRedHardDescriptor : EnemyDescriptor
	{
		sf::Vector2f speed{ .0f, .0f };
	};

	bool init(const SlimeRedHardDescriptor& enemyDescriptor);

	sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

	void update(float deltaMilliseconds) override;
	void render(sf::RenderWindow& window) override;

	sf::FloatRect getBounds();

private:

	sf::Vector2f m_direction{ .0f, .0f };
	sf::Vector2f m_speed{ 20.0f, 20.0f };

	float m_boundsOffsetX{ 12.f };
	float m_boundsOffsetY{ 12.f };
};