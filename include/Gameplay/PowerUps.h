#pragma once

#include <iostream>
#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

class PowerUps : public GameObject
{
public:

	PowerUps() { m_sprite.setScale(0.75f, 0.75f); }

	struct PowerUpsDescriptor
	{
		sf::Vector2f position{ 0.f, 0.f };
		sf::Texture* texture{ nullptr };
		float tileWidth{ .0f };
		float tileHeight{ .0f };
	};

	bool init(const PowerUpsDescriptor& powerUpsDescriptor);

	sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

	void update(float deltaMilliseconds) override;
	void render(sf::RenderWindow& window) override;

	int m_scoreAddedPerCoin{ 25 };
	int m_healthAddedPerHeart{ 1 };
	float m_enegyAddedWhenPicked{ 1.5f };

	void setActive(bool isActive) { m_isActive = isActive; };
	bool isActive() const { return m_isActive; }

	int id{ 0 };	//id = 1 -> coin		//id = 2 -> heart		// id = 3 -> energy
	int getID() { return id; }

	void setTimeExpired(bool hasExpired) { m_timeExpired = hasExpired; }
	bool getTimeExpired() const { return m_timeExpired; }
	float m_time = { 0.0f };
	int m_previousSecond{ 0 };
	void resetTimers();

protected:

	sf::Sprite m_sprite;
	float m_tileWidth{ .0f };
	float m_tileHeight{ .0f };

	bool m_isActive{ true };

private:
	float m_millisecondsToSeconds{ 1 / 1000.f };
	int m_timeToDisappear{ 5 };
	bool m_timeExpired{ false };

};