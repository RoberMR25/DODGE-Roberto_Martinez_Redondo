#include <Gameplay/Zombie.h>
#include <SFML/Window/Keyboard.hpp>

// =============================================================================================
// DISABLED (Not eliminated just in case there's a need to check something originally made here)
// =============================================================================================

bool Zombie::init(const ZombieDescriptor& zombieDescriptor)
{
	m_speed = zombieDescriptor.speed;
	
	return Enemy::init(zombieDescriptor);
}

void Zombie::update(float deltaMilliseconds)
{
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	//	m_direction.x = -1.0f;
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	//	m_direction.x = 1.0f;
	//else
	//	m_direction.x = .0f;

	//m_position.x += (m_direction.x * m_speed.x * deltaMilliseconds);
	//m_position.y += (m_direction.y * m_speed.y * deltaMilliseconds);

	Enemy::update(deltaMilliseconds);
}