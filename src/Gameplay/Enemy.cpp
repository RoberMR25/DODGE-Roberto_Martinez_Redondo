#include <Gameplay/Enemy.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

bool Enemy::init(const EnemyDescriptor& enemyDescriptor)
{
	m_sprite.setTexture(*enemyDescriptor.texture);
	m_sprite.setPosition(enemyDescriptor.position);
	m_direction = enemyDescriptor.direction;
	m_tileWidth = enemyDescriptor.tileWidth;
	m_tileHeight = enemyDescriptor.tileHeight;
	
	return true;
}

void Enemy::update(float deltaMilliseconds)
{
	m_sprite.setPosition(m_position);
}

void Enemy::render(sf::RenderWindow& window)
{
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_tileWidth, m_tileHeight));
	window.draw(m_sprite);
}