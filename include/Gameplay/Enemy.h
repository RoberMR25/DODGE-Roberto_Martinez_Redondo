#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Enemy : public GameObject
{
	public:

		struct EnemyDescriptor
		{
			sf::Vector2f position{ .0f, .0f };
			sf::Vector2f direction{ .0f, .0f };
			sf::Texture* texture{ nullptr };
			float tileWidth{ .0f };
			float tileHeight{ .0f };
		};

		~Enemy() override = default;

		bool init(const EnemyDescriptor& enemyDescriptor);

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

		int m_damage{ 0 };

		void setActive(bool isActive) { m_isActive = isActive; };
		bool isActive() const { return m_isActive; }

		bool m_isInCollision = false;

	protected:

		sf::Vector2f m_direction{ .0f, .0f };
		sf::Sprite m_sprite;
		float m_tileWidth{ 45.0f };
		float m_tileHeight{ 47.0f };
		sf::Color m_color;

		bool m_isActive{ true };
};