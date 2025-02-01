#include <Core/AssetManager.h>
#include <Core/World.h>

#include <Gameplay/Player.h>

#include <random>

#include <Render/SFMLOrthogonalLayer.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <SFX/AudioManager.h>

#include <tmxlite/Map.hpp>

World::World()
{
	m_millisecondsToSeconds = 1 / 1000.f;

	m_collisionOffset = -37.0f;
	m_hasInitiatedCollision = false;
	m_hasInitiatedCollisionWithPowerUp = false;

	m_playerScore = 0;
	m_scoreAddedPerSecond = 5;
	m_time = 0.0f;
	m_previousSecond = 0;

	m_timeForPowerUpsToSpawn = 5;
}

World::~World()	// When World is destroyed...
{
	// ...we make sure to clean all the Pools...
	if (m_slimeBlueEasyEnemies.size() > 0)
		for (int i = 0; i < m_slimeBlueEasyEnemies.size(); i++)
			m_slimeBlueEasyObjectPool.release(*dynamic_cast<SlimeBlueEasy*>(m_slimeBlueEasyEnemies[i]));

	if (m_slimeGreenMediumEnemies.size() > 0)
		for (int i = 0; i < m_slimeGreenMediumEnemies.size(); i++)
			m_slimeGreenMediumObjectPool.release(*dynamic_cast<SlimeGreenMedium*>(m_slimeGreenMediumEnemies[i]));

	if (m_slimeRedHardEnemies.size() > 0)
		for (int i = 0; i < m_slimeRedHardEnemies.size(); i++)
			m_slimeRedHardObjectPool.release(*dynamic_cast<SlimeRedHard*>(m_slimeRedHardEnemies[i]));

	if (m_powerUpsVector.size() > 0)
		for (int i = 0; i < m_powerUpsVector.size(); i++)
			m_powerUpsObjectPool.release(*dynamic_cast<PowerUps*>(m_powerUpsVector[i]));

	// ...and we delete all the layers, the audio and the map 
	delete m_layerZero;
	delete m_layerOne;
	delete m_layerTwo;

	delete m_groundCollisionLayer;
	delete m_leftWallCollisionLayer;
	delete m_rightWallCollisionLayer;
	delete m_topCollisionLayer;
	delete m_enemyDespawnCollisionLayer;

	delete m_audioManager;

	delete m_map;
}

bool World::load()
{
	// Map loading
	m_map = new tmx::Map();
	m_map->load("../data/Levels/level_v1.tmx");

	// Layers loading
	m_layerZero = new MapLayer(*m_map, 0);
	m_layerOne = new MapLayer(*m_map, 1);
	m_layerTwo = new MapLayer(*m_map, 2);

	m_groundCollisionLayer = new ObjectLayer(*m_map, 3);
	m_leftWallCollisionLayer = new ObjectLayer(*m_map, 4);
	m_rightWallCollisionLayer = new ObjectLayer(*m_map, 5);
	m_topCollisionLayer = new ObjectLayer(*m_map, 6);
	m_enemyDespawnCollisionLayer = new ObjectLayer(*m_map, 7);

	// Player instantiation
	sf::Texture* playerTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Player/player.png");
	Player::PlayerDescriptor playerDescriptor;
	playerDescriptor.texture = playerTexture;
	playerDescriptor.position = { 0.f, 0.f };
	playerDescriptor.defaultSpeed = { 200.f * m_millisecondsToSeconds, 200.f * m_millisecondsToSeconds };
	playerDescriptor.sprintSpeed = { 400.f * m_millisecondsToSeconds, 400.f * m_millisecondsToSeconds };
	playerDescriptor.speed = { playerDescriptor.defaultSpeed };
	playerDescriptor.tileWidth = 64.f;
	playerDescriptor.tileHeight = 64.f;
	Player* player = new Player();
	const bool initOk = player->init(playerDescriptor);

	m_player = player;
	player->setPosition({ 192.f, 113.f });	// Center of the scenery

	// Audio Manager instantiation
	m_audioManager = new AudioManager();
	m_audioManager->setVolume(80.f);

	return initOk;
}

void World::update(uint32_t deltaMilliseconds)
{
	m_layerZero->update(sf::milliseconds(deltaMilliseconds));

	if (m_player->getHealth() > 0)	// Time and score stop when player dies
		updateTimer(deltaMilliseconds);

	// Update Enemies
	if (m_slimeBlueEasyEnemies.size() > 0)
		for (int i = 0; i < m_slimeBlueEasyEnemies.size(); i++)
			m_slimeBlueEasyEnemies[i]->update(deltaMilliseconds);

	if (m_slimeGreenMediumEnemies.size() > 0)
		for (int i = 0; i < m_slimeGreenMediumEnemies.size(); i++)
			m_slimeGreenMediumEnemies[i]->update(deltaMilliseconds);

	if (m_slimeRedHardEnemies.size() > 0)
		for (int i = 0; i < m_slimeRedHardEnemies.size(); i++)
			m_slimeRedHardEnemies[i]->update(deltaMilliseconds);

	// Update Player
	m_player->update(deltaMilliseconds);

	// Update PowerUps
	if (m_powerUpsVector.size() > 0)
		for (int i = 0; i < m_powerUpsVector.size(); i++)
			m_powerUpsVector[i]->update(deltaMilliseconds);

	// Check for collisions: in here, we check if Player collides with any of the 4 walls (left wall, right wall, roof or ground)
	// If it does, depending on which he has collided with, we modify the player's position so that he doesn't cross them
	const auto& groundCollisionShapes = m_groundCollisionLayer->getShapes();	// If he collides with the ground...
	for (const auto* shape : groundCollisionShapes)
		if (shape->getGlobalBounds().intersects(m_player->getBounds()))
			m_player->setPosition({ m_player->getPosition().x, shape->getGlobalBounds().top - m_player->getBounds().height + m_collisionOffset });

	const auto& topCollisionShapes = m_topCollisionLayer->getShapes();	// If he collides with the roof...
	for (const auto* shape : topCollisionShapes)
		if (shape->getGlobalBounds().intersects(m_player->getBounds()))
			m_player->setPosition({ m_player->getPosition().x, shape->getGlobalBounds().top + shape->getGlobalBounds().height + m_collisionOffset });

	const auto& leftWallCollisionShapes = m_leftWallCollisionLayer->getShapes();	// If he collides with the left wall...
	for (const auto* shape : leftWallCollisionShapes)
		if (shape->getGlobalBounds().intersects(m_player->getBounds()))
			m_player->setPosition({ shape->getGlobalBounds().left + shape->getGlobalBounds().width + m_collisionOffset, m_player->getPosition().y });

	const auto& rightWallCollisionShapes = m_rightWallCollisionLayer->getShapes();	// If he collides with the right wall...
	for (const auto* shape : rightWallCollisionShapes)
		if (shape->getGlobalBounds().intersects(m_player->getBounds()))
			m_player->setPosition({ shape->getGlobalBounds().left - m_player->getBounds().width + m_collisionOffset, m_player->getPosition().y });

	checkCollision();				// We check Enemies and PowerUps collisions with the player
	checkCollisionForEnemies();		// We check Enemies collisions with the outer colliders of the scenery (so that they deactivate when exit the map)
	checkPowerUpsForDespawn();		// We check the time that a PowerUp has been active, since it will eventually be deactivated if it's not picked
}

void World::render(sf::RenderWindow& window)
{
	window.draw(*m_layerZero);
	window.draw(*m_layerOne);
	window.draw(*m_layerTwo);

	// Visual escenery colliders
	/*
		window.draw(*m_groundCollisionLayer);
		window.draw(*m_leftWallCollisionLayer);
		window.draw(*m_rightWallCollisionLayer);
		window.draw(*m_topCollisionLayer);
		window.draw(*m_enemyDespawnCollisionLayer);
	*/

	// Active PowerUps render
	if (m_powerUpsVector.size() > 0)
		for (int i = 0; i < m_powerUpsVector.size(); i++)
			if (m_powerUpsVector[i]->isActive())
				m_powerUpsVector[i]->render(window);

	// Active Enemies render
	if (m_slimeBlueEasyEnemies.size() != 0)
		for (int i = 0; i < m_slimeBlueEasyEnemies.size(); i++)
			if (m_slimeBlueEasyEnemies[i]->isActive())
				m_slimeBlueEasyEnemies[i]->render(window);

	if (m_slimeGreenMediumEnemies.size() != 0)
		for (int i = 0; i < m_slimeGreenMediumEnemies.size(); i++)
			if (m_slimeGreenMediumEnemies[i]->isActive())
				m_slimeGreenMediumEnemies[i]->render(window);

	if (m_slimeRedHardEnemies.size() != 0)
		for (int i = 0; i < m_slimeRedHardEnemies.size(); i++)
			if (m_slimeRedHardEnemies[i]->isActive())
				m_slimeRedHardEnemies[i]->render(window);

	// Player render
	m_player->render(window);
}

void World::checkCollision()	// This way the collision makes its effect only once, instead of every frame, until i stops colliding
{
	if (m_slimeBlueEasyEnemies.size() > 0)	// Blue (easy) Enemy collision with Player (if there are this kind of enemies active)
		for (int i = 0; i < m_slimeBlueEasyEnemies.size();)
		{
			if (m_slimeBlueEasyEnemies[i]->getBounds().intersects(m_player->getBounds()) && m_slimeBlueEasyEnemies[i]->isActive())
			{
				chooseHealthLost(m_slimeBlueEasyEnemies[i]->m_damage);  // m_slimeBlueEasyEnemies[i]->m_damage = 1 (easy enemy)
				m_slimeBlueEasyEnemies[i]->m_isInCollision = true;		// We mark it as inCollision, so that it only collides once
				m_slimeBlueEasyObjectPool.release(*m_slimeBlueEasyEnemies[i]);		// We release it from the Pool, so that it can be used again
				m_slimeBlueEasyEnemies[i]->setActive(false);			// We deactivate it
				m_slimeBlueEasyEnemies.erase(m_slimeBlueEasyEnemies.begin() + i);	// We erase it from the managing list of enemies
			}
			if (i < m_slimeBlueEasyEnemies.size() && !m_slimeBlueEasyEnemies[i]->m_isInCollision)
				++i;
		}

	if (m_slimeGreenMediumEnemies.size() > 0)	// Green (medium) Enemy collision with Player (if there are this kind of enemies active)
		for (int i = 0; i < m_slimeGreenMediumEnemies.size();)
		{
			if (m_slimeGreenMediumEnemies[i]->getBounds().intersects(m_player->getBounds()) && m_slimeGreenMediumEnemies[i]->isActive())
			{
				chooseHealthLost(m_slimeGreenMediumEnemies[i]->m_damage); // m_slimeGreenMediumEnemies[i]->m_damage = 2 (medium enemy)
				m_slimeGreenMediumEnemies[i]->m_isInCollision = true;
				m_slimeGreenMediumObjectPool.release(*m_slimeGreenMediumEnemies[i]);
				m_slimeGreenMediumEnemies[i]->setActive(false);
				m_slimeGreenMediumEnemies.erase(m_slimeGreenMediumEnemies.begin() + i);
			}
			if (i < m_slimeGreenMediumEnemies.size() && !m_slimeGreenMediumEnemies[i]->m_isInCollision)
				++i;
		}

	if (m_slimeRedHardEnemies.size() > 0)	// Red (hard) Enemy collision with Player (if there are this kind of enemies active)
		for (int i = 0; i < m_slimeRedHardEnemies.size();)
		{
			if (m_slimeRedHardEnemies[i]->getBounds().intersects(m_player->getBounds()) && m_slimeRedHardEnemies[i]->isActive())
			{
				chooseHealthLost(m_slimeRedHardEnemies[i]->m_damage); // m_slimeRedHardEnemies[i]->m_damage = 3 (hard enemy)
				m_slimeRedHardEnemies[i]->m_isInCollision = true;
				m_slimeRedHardObjectPool.release(*m_slimeRedHardEnemies[i]);
				m_slimeRedHardEnemies[i]->setActive(false);
				m_slimeRedHardEnemies.erase(m_slimeRedHardEnemies.begin() + i);
			}
			if (i < m_slimeRedHardEnemies.size() && !m_slimeRedHardEnemies[i]->m_isInCollision)
				++i;
		}

	if (m_powerUpsVector.size() > 0)	// PowerUps collision with Player (if there are active PowerUps)
		for (int i = 0; i < m_powerUpsVector.size();)
		{
			if (m_powerUpsVector[i]->getBounds().intersects(m_player->getBounds()) && m_powerUpsVector[i]->isActive())
			{
				if (m_powerUpsVector[i]->getID() == 1)	// Coin
				{
					m_audioManager->playSound(2);	// playSound(2) = Coin sound
					setScoreByCoin(m_powerUpsVector[i]->m_scoreAddedPerCoin);
				}
				else if (m_powerUpsVector[i]->getID() == 2)	// Heart
				{
					m_audioManager->playSound(3);	// playSound(3) = Heart sound
					m_player->recoverHealth(m_powerUpsVector[i]->m_healthAddedPerHeart);
				}
				else if (m_powerUpsVector[i]->getID() == 3)	// Energy
				{
					m_audioManager->playSound(4);	// playSound(4) = Energy sound
					m_player->recoverEnergy(m_powerUpsVector[i]->m_enegyAddedWhenPicked);
				}

				m_powerUpsObjectPool.release(*m_powerUpsVector[i]);
				m_powerUpsVector[i]->setActive(false);
				m_powerUpsVector.erase(m_powerUpsVector.begin() + i);
			}
			if (i < m_powerUpsVector.size())
				++i;
		}
	else
		m_hasInitiatedCollision = false;
}

void World::checkCollisionForEnemies()	// If any of the enemies collide with m_enemyDespawnCollisionLayer, it despawns and gets back to the Pool
{										// the exact same way it happened when they collided with the player (obviously without making damage)
	if (m_slimeBlueEasyEnemies.size() > 0)
		for (int i = 0; i < m_slimeBlueEasyEnemies.size();)
		{
			const auto& enemyDespawnCollisionShapes = m_enemyDespawnCollisionLayer->getShapes();
			for (const auto* shape : enemyDespawnCollisionShapes)
				if (shape->getGlobalBounds().intersects(m_slimeBlueEasyEnemies[i]->getBounds()) && m_slimeBlueEasyEnemies[i]->m_isInCollision == false)
				{
					m_slimeBlueEasyEnemies[i]->m_isInCollision = true;
					m_slimeBlueEasyObjectPool.release(*m_slimeBlueEasyEnemies[i]);
					m_slimeBlueEasyEnemies[i]->setActive(false);
					m_slimeBlueEasyEnemies.erase(m_slimeBlueEasyEnemies.begin() + i);
					break;
				}
			if (i < m_slimeBlueEasyEnemies.size() && !m_slimeBlueEasyEnemies[i]->m_isInCollision)
				++i;
		}

	if (m_slimeGreenMediumEnemies.size() > 0)
		for (int i = 0; i < m_slimeGreenMediumEnemies.size();)
		{
			const auto& enemyDespawnCollisionShapes = m_enemyDespawnCollisionLayer->getShapes();
			for (const auto* shape : enemyDespawnCollisionShapes)
				if (shape->getGlobalBounds().intersects(m_slimeGreenMediumEnemies[i]->getBounds()) && m_slimeGreenMediumEnemies[i]->m_isInCollision == false)
				{
					m_slimeGreenMediumEnemies[i]->m_isInCollision = true;
					m_slimeGreenMediumObjectPool.release(*m_slimeGreenMediumEnemies[i]);
					m_slimeGreenMediumEnemies[i]->setActive(false);
					m_slimeGreenMediumEnemies.erase(m_slimeGreenMediumEnemies.begin() + i);
					break;
				}
			if (i < m_slimeGreenMediumEnemies.size() && !m_slimeGreenMediumEnemies[i]->m_isInCollision)
				++i;
		}

	if (m_slimeRedHardEnemies.size() > 0)
		for (int i = 0; i < m_slimeRedHardEnemies.size();)
		{
			const auto& enemyDespawnCollisionShapes = m_enemyDespawnCollisionLayer->getShapes();
			for (const auto* shape : enemyDespawnCollisionShapes)
				if (shape->getGlobalBounds().intersects(m_slimeRedHardEnemies[i]->getBounds()) && m_slimeRedHardEnemies[i]->m_isInCollision == false)
				{
					m_slimeRedHardEnemies[i]->m_isInCollision = true;
					m_slimeRedHardObjectPool.release(*m_slimeRedHardEnemies[i]);
					m_slimeRedHardEnemies[i]->setActive(false);
					m_slimeRedHardEnemies.erase(m_slimeRedHardEnemies.begin() + i);
					break;
				}
			if (i < m_slimeRedHardEnemies.size() && !m_slimeRedHardEnemies[i]->m_isInCollision)
				++i;
		}
}

void World::checkPowerUpsForDespawn()	// If an active Power Ups time has expired, it deactivated and gets back to the Pool
{
	if (m_powerUpsVector.size() > 0)
		for (int i = 0; i < m_powerUpsVector.size();)
		{
			if (m_powerUpsVector[i]->getTimeExpired())
			{
				m_powerUpsObjectPool.release(*m_powerUpsVector[i]);
				m_powerUpsVector[i]->setActive(false);
				m_powerUpsVector.erase(m_powerUpsVector.begin() + i);
			}
			if (i < m_powerUpsVector.size())
				++i;
		}
}

void World::chooseHealthLost(int playerHealthLost)
{
	if (!m_hasInitiatedCollision)	// So that it doesn't work once per frame while the collision is active, but just once at all
	{
		m_audioManager->playSound(1);	// playSound(1) = Enemy Hurt Sound
		m_hasInitiatedCollision = true;
		m_player->loseHealth(playerHealthLost);
	}
}

void World::setScoreByCoin(int scoreAdded)
{
	++m_coinsCollected;	// For the interface
	m_playerScore += scoreAdded;
}

void World::updateTimer(uint32_t deltaMillisecondsForTimer)
{
	m_time += deltaMillisecondsForTimer * m_millisecondsToSeconds;	// Update time
	int seconds = static_cast<int>(m_time);		// For the timer: cast float (time) into int, so that it doesn't show decimals
	if (seconds > m_previousSecond)				// For the timer: Only updates seconds once per second (not every frame)
	{
		m_previousSecond = seconds;
		setScoreByTimer();	// Score increases each second survived

		if (seconds % m_timeForPowerUpsToSpawn == 0)  // Every "m_timeForPowerUpsToSpawn" seconds, a Power Up spawns
			powerUpsSpawn();
	}

	// Here we make that difficulty changes from easy to medium when player has 150 points, and from medium to hard when player has 300 points
	// This happens fast, and it's made this way so it can be easily tested. We could modify this values to make it longer to get to higher difficulties
	if (m_playerScore < m_scoreRequiredToChangeFromEasyToMediumDifficulty)	// When player's score is less than 150...
	{
		m_enemySpawnTime = m_enemySpawnTimeEasyDif;	// ...enemies spawn every 0.75 seconds...

		m_blueEnemySpeed = m_blueSlimeSpeed_EasyDif;	// ...and they have less speed
		m_greenEnemySpeed = m_greenSlimeSpeed_EasyDif;
		m_redEnemySpeed = m_redSlimeSpeed_EasyDif;
	}
	else if (m_playerScore >= m_scoreRequiredToChangeFromEasyToMediumDifficulty && m_playerScore < m_scoreRequiredToChangeFromMediumToHardDifficulty)
	{													// When player's score is between 150 and 300... 
		m_enemySpawnTime = m_enemySpawnTimeMediumDif;	// ...enemies spawn every 0.5 seconds...

		m_blueEnemySpeed = m_blueSlimeSpeed_MediumDif;	// ...and they have average speed
		m_greenEnemySpeed = m_greenSlimeSpeed_MediumDif;
		m_redEnemySpeed = m_redSlimeSpeed_MediumDif;
	}
	else if (m_playerScore >= m_scoreRequiredToChangeFromMediumToHardDifficulty)	// When player's score is higher than 300...
	{
		m_enemySpawnTime = m_enemySpawnTimeHardDif;		// ...enemies spawn each 0.25 seconds...

		m_blueEnemySpeed = m_blueSlimeSpeed_HardDif;	// ...and they have higher speed
		m_greenEnemySpeed = m_greenSlimeSpeed_HardDif;
		m_redEnemySpeed = m_redSlimeSpeed_HardDif;
	}

	// This way, 1 enemy spawns every "m_enemySpawnTime" seconds
	static float timerForEnemySpawn = 0.0f;
	timerForEnemySpawn += deltaMillisecondsForTimer * m_millisecondsToSeconds;
	if (timerForEnemySpawn >= m_enemySpawnTime)
	{
		enemySpawn();
		timerForEnemySpawn -= m_enemySpawnTime;
	}
}

void World::enemySpawn()
{
	float randomNumber = floatRandomRange(0.0f, 1.0f);

	if (randomNumber >= 0.f && randomNumber < 0.5f)	// 50% probabilities for a slimeBlueEasy Enemy to spawn
		slimeBlueEasySpawn();
	else if (randomNumber >= 0.5f && randomNumber < 0.8f)	// 30% probabilities for a slimeGreenMedium Enemy to spawn
		slimeGreenMediumSpawn();
	else if (randomNumber >= 0.8f && randomNumber <= 1.0f)	// 20% probabilities for a slimeRedHard Enemy to spawn
		slimeRedHardSpawn();
}

void World::slimeBlueEasySpawn()
{
	SlimeBlueEasy& slimeBlueEasy = m_slimeBlueEasyObjectPool.get();	// We get one enemy of this type from its Pool
	slimeBlueEasy.m_isInCollision = false;	// We declare that it is not in collision at first

	SlimeBlueEasy::SlimeBlueEasyDescriptor slimeBlueEasyDescriptor;	// We access to its descriptor...
	// ...and set all its variables
	sf::Texture* slimeBlueEasyTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Enemies/enemy01_darker.png");	// We load its texture...
	slimeBlueEasyDescriptor.texture = slimeBlueEasyTexture;	// ...and set it
	slimeBlueEasyDescriptor.position = { 0.f, 0.f };
	slimeBlueEasyDescriptor.speed = m_blueEnemySpeed;
	sf::Vector2f enemySpawnDirection = { floatRandomRange(-0.9f, 0.9f), floatRandomRange(-0.9f, 0.9f) }; // We give it a direction, not from (-1.0f, 1.0f)
	slimeBlueEasyDescriptor.direction = { enemySpawnDirection };										 // so that it doesn't go absolutely straight
	slimeBlueEasyDescriptor.tileWidth = 45.f;
	slimeBlueEasyDescriptor.tileHeight = 47.f;
	slimeBlueEasy.setActive(true);	// We activate it... 
	slimeBlueEasy.init(slimeBlueEasyDescriptor); // ...and call init()

	sf::Vector2f enemySpawnPosition = setEnemySpawnPosition(enemySpawnDirection);	// We choose the enemies' spawn position...
	slimeBlueEasy.setPosition(enemySpawnPosition);	// ...and set it
	m_slimeBlueEasyEnemies.push_back(&slimeBlueEasy);	// And we add it to the managing list
}

void World::slimeGreenMediumSpawn()	// This works exactly the same as the previous one
{
	SlimeGreenMedium& slimeGreenMedium = m_slimeGreenMediumObjectPool.get();
	slimeGreenMedium.m_isInCollision = false;

	SlimeGreenMedium::SlimeGreenMediumDescriptor slimeGreenMediumDescriptor;
	sf::Texture* slimeGreenMediumTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Enemies/enemy02_darker.png");
	slimeGreenMediumDescriptor.texture = slimeGreenMediumTexture;
	slimeGreenMediumDescriptor.position = { 0.f, 0.f };
	slimeGreenMediumDescriptor.speed = m_greenEnemySpeed;
	sf::Vector2f enemySpawnDirection = { floatRandomRange(-0.9f, 0.9f), floatRandomRange(-0.9f, 0.9f) };
	slimeGreenMediumDescriptor.direction = { enemySpawnDirection };
	slimeGreenMediumDescriptor.tileWidth = 45.f;
	slimeGreenMediumDescriptor.tileHeight = 47.f;
	slimeGreenMedium.setActive(true);
	slimeGreenMedium.init(slimeGreenMediumDescriptor);

	sf::Vector2f enemySpawnPosition = setEnemySpawnPosition(enemySpawnDirection);
	slimeGreenMedium.setPosition(enemySpawnPosition);
	m_slimeGreenMediumEnemies.push_back(&slimeGreenMedium);
}

void World::slimeRedHardSpawn()	// This works exactly the same as the first one
{
	SlimeRedHard& slimeRedHard = m_slimeRedHardObjectPool.get();
	slimeRedHard.m_isInCollision = false;

	SlimeRedHard::SlimeRedHardDescriptor slimeRedHardDescriptor;
	sf::Texture* slimeRedHardTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Enemies/enemy03_darker.png");
	slimeRedHardDescriptor.texture = slimeRedHardTexture;
	slimeRedHardDescriptor.position = { 0.f, 0.f };
	slimeRedHardDescriptor.speed = m_redEnemySpeed;
	sf::Vector2f enemySpawnDirection = { floatRandomRange(-0.9f, 0.9f), floatRandomRange(-0.9f, 0.9f) };
	slimeRedHardDescriptor.direction = { enemySpawnDirection };
	slimeRedHardDescriptor.tileWidth = 45.f;
	slimeRedHardDescriptor.tileHeight = 47.f;
	slimeRedHard.setActive(true);
	slimeRedHard.init(slimeRedHardDescriptor);

	sf::Vector2f enemySpawnPosition = setEnemySpawnPosition(enemySpawnDirection);
	slimeRedHard.setPosition(enemySpawnPosition);
	m_slimeRedHardEnemies.push_back(&slimeRedHard);
}

sf::Vector2f World::setEnemySpawnPosition(sf::Vector2f enemyDirection)
{
	// This is quite a difficult function to explain. 
	// Basically I wanted to choose a spawn position depending on the direction they were given when instantiated (a random direction)
	// so that if, for example, they are going (based on their direction) to the up-right corner, they have to spawn at the down-left corner,
	// to avoid moving in the outsides of the map. But I didn't want them to just go on four posible routes, so I wanted to randomize their x and y position.
	// But that generates a problem that, if x and y are concrete values, the enemies can spawn in the middle of the map, so I had to make sure
	// that, depending on how "x" they appear, their "y" would has more or less freedom. 
	// But also, that would mean that "y" would depend always on "x", resulting in that most of the times enemies would came from above and below and
	// not from left or right. So I made a random number, called "dependency", where if it is 1, "y" will depend on "x" (so we first set "x" and then, depending
	// on "x" value, we look for a value to "y", and if dependency is 2, it is "x" that will depend on "y".

	// It is quite complicated, but it basically achieves that enemies can appear anywhere in the limits of the map,
	// without appearing in the middle, and they can go on different directions but inside the map.

	sf::Vector2f enemySpawnPosition;
	int dependency = intRandomRange(1, 2);
	// if dependency == 1, the enemySpawnPosition.y will depend on the enemySpawnPosition.x
	// if dependency == 2, the enemySpawnPosition.x will depend on the enemySpawnPosition.y

	// Up Left: { -40.f, -40.f }		{ 1.0f, 1.0f }
	// Up Right: { 520.f, -40.f }		{ -1.0f, 1.0f }
	// Down Left: { -40.f, 360.f }		{ 1.0f, -1.0f }
	// Down Right: { 520.f, 360.f }		{ -1.0f, -1.0f }

	if (dependency == 1)	// If "y" depends on "x"
	{	// We firstly set "x"
		if (enemyDirection.x < 0.9f && enemyDirection.x >= 0.0f)
			enemySpawnPosition.x = floatRandomRange(-40.f, 240.f);
		else if (enemyDirection.x < 0.0f)
			enemySpawnPosition.x = floatRandomRange(240.f, 520.f);

		if (enemyDirection.y < 0.9f && enemyDirection.y >= 0.0f) // And then, depending on direction and "x" position, we set "y"
		{	
			if (enemySpawnPosition.x >= -40.f && enemySpawnPosition.x < 240.f)
			{
				if (enemySpawnPosition.x > -40.f && enemySpawnPosition.x <= -25.f)
					enemySpawnPosition.y = floatRandomRange(-40.f, 160.f);
				else if (enemySpawnPosition.x > -25.f)
					enemySpawnPosition.y = -40.f;
			}
			else if (enemySpawnPosition.x >= 240.f)
			{
				if (enemySpawnPosition.x < 520.f && enemySpawnPosition.x >= 505.f)
					enemySpawnPosition.y = floatRandomRange(-40.f, 160.f);
				else if (enemySpawnPosition.x < 505.f)
					enemySpawnPosition.y = -40.f;
			}
		}
		else if (enemyDirection.y < 0.0f)
		{
			if (enemySpawnPosition.x >= -40.f && enemySpawnPosition.x < 240.f)
			{
				if (enemySpawnPosition.x > -40.f && enemySpawnPosition.x <= -25.f)
					enemySpawnPosition.y = floatRandomRange(160.f, 360.f);
				else if (enemySpawnPosition.x > -25.f)
					enemySpawnPosition.y = 360.f;
			}
			else if (enemySpawnPosition.x >= 240.f)
			{
				if (enemySpawnPosition.x < 520.f && enemySpawnPosition.x >= 505.f)
					enemySpawnPosition.y = floatRandomRange(160.f, 360.f);
				else if (enemySpawnPosition.x < 505.f)
					enemySpawnPosition.y = 360.0f;
			}
		}
	}
	else if (dependency == 2)	// If "x" depends on "y"
	{	// We firstly set "y"
		if (enemyDirection.y < 0.9f && enemyDirection.y >= 0.0f) // And then, depending on direction and "y" position, we set "x"
			enemySpawnPosition.y = floatRandomRange(-40.f, 160.f);
		else if (enemyDirection.y < 0.0f)
			enemySpawnPosition.y = floatRandomRange(160.f, 360.f);

		if (enemyDirection.x < 0.9f && enemyDirection.x >= 0.0f)
		{
			if (enemySpawnPosition.y >= -40.f && enemySpawnPosition.y < 160.f)
			{
				if (enemySpawnPosition.y > -40.f && enemySpawnPosition.y <= -25.f)
					enemySpawnPosition.x = floatRandomRange(-40.f, 240.f);
				else if (enemySpawnPosition.y > -25.f)
					enemySpawnPosition.x = -40.f;
			}
			else if (enemySpawnPosition.y >= 160.f)
			{
				if (enemySpawnPosition.y < 360.f && enemySpawnPosition.y >= 345.f)
					enemySpawnPosition.x = floatRandomRange(-40.f, 240.f);
				else if (enemySpawnPosition.y < 345.f)
					enemySpawnPosition.x = -40.f;
			}
		}
		else if (enemyDirection.x < 0.0f)
		{
			if (enemySpawnPosition.y >= -40.f && enemySpawnPosition.y < 160.f)
			{
				if (enemySpawnPosition.y > -40.f && enemySpawnPosition.y <= -25.f)
					enemySpawnPosition.x = floatRandomRange(240.f, 520.f);
				else if (enemySpawnPosition.y > -25.f)
					enemySpawnPosition.x = 520.f;
			}
			else if (enemySpawnPosition.y >= 160.f)
			{
				if (enemySpawnPosition.y < 360.f && enemySpawnPosition.y >= 345.f)
					enemySpawnPosition.x = floatRandomRange(240.f, 520.f);
				else if (enemySpawnPosition.y < 345.f)
					enemySpawnPosition.x = 520.f;
			}
		}
	}

	return enemySpawnPosition;
}

void World::powerUpsSpawn()
{
	float randomNumber = floatRandomRange(0.f, 1.f);
	if (randomNumber >= 0.f && randomNumber < 0.6f)			// 60% probabilities of a coin spawning 
		coinsSpawn();
	else if (randomNumber >= 0.6f && randomNumber < 0.8f)	// 20% probabilities of a heart spawning 
		heartsSpawn();
	else if (randomNumber >= 0.8f && randomNumber <= 1.0f)	// 20% probabilities of an energy spawning 
		energySpawn();
}

void World::coinsSpawn()	// Same process that when spawning enemies, but with the PowerUps characteristics
{
	PowerUps& coin = m_powerUpsObjectPool.get();

	PowerUps::PowerUpsDescriptor coinsDescriptor;
	sf::Texture* coinsTexture = AssetManager::getInstance()->loadTexture("../Data/Images/PowerUps/coin.png");
	coinsDescriptor.texture = coinsTexture;
	coinsDescriptor.position = { 0.f, 0.f };
	coinsDescriptor.tileWidth = 32.f;
	coinsDescriptor.tileHeight = 32.f;
	coin.setActive(true);
	coin.init(coinsDescriptor);

	sf::Vector2f powerUpsSpawnPosition = { floatRandomRange(45.f, 435.f), floatRandomRange(45.f, 275.f) };
	coin.setPosition(powerUpsSpawnPosition);

	m_powerUpsVector.push_back(&coin);

	coin.id = 1;
	coin.setTimeExpired(false);
}

void World::heartsSpawn()	// Same process that when spawning enemies, but with the PowerUps characteristics
{
	PowerUps& heart = m_powerUpsObjectPool.get();

	PowerUps::PowerUpsDescriptor heartDescriptor;
	sf::Texture* heartTexture = AssetManager::getInstance()->loadTexture("../Data/Images/PowerUps/heart.png");
	heartDescriptor.texture = heartTexture;
	heartDescriptor.position = { 0.f, 0.f };
	heartDescriptor.tileWidth = 32.f;
	heartDescriptor.tileHeight = 32.f;
	heart.setActive(true);
	heart.init(heartDescriptor);

	sf::Vector2f powerUpsSpawnPosition = { floatRandomRange(45.f, 435.f), floatRandomRange(45.f, 275.f) };
	heart.setPosition(powerUpsSpawnPosition);

	m_powerUpsVector.push_back(&heart);

	heart.id = 2;
	heart.setTimeExpired(false);
}

void World::energySpawn()
{
	PowerUps& energy = m_powerUpsObjectPool.get();	// Same process that when spawning enemies, but with the PowerUps characteristics

	PowerUps::PowerUpsDescriptor energyDescriptor;
	sf::Texture* energyTexture = AssetManager::getInstance()->loadTexture("../Data/Images/PowerUps/energy.png");
	energyDescriptor.texture = energyTexture;
	energyDescriptor.position = { 0.f, 0.f };
	energyDescriptor.tileWidth = 32.f;
	energyDescriptor.tileHeight = 32.f;
	energy.setActive(true);
	energy.init(energyDescriptor);

	sf::Vector2f powerUpsSpawnPosition = { floatRandomRange(45.f, 435.f), floatRandomRange(45.f, 275.f) };
	energy.setPosition(powerUpsSpawnPosition);

	m_powerUpsVector.push_back(&energy);

	energy.id = 3;
	energy.resetTimers();
}

float World::floatRandomRange(float min, float max)
{
	// This emulates de UnityEngine.RandomRange() that I know from C#. In this case, for floats
	std::random_device randomSeed;
	std::mt19937 mt(randomSeed());
	std::uniform_real_distribution<float> randomNumber(min, max);
	return randomNumber(mt);
}

int World::intRandomRange(int min, int max)
{
	// This emulates de UnityEngine.RandomRange() that I know from C#. In this case, for ints
	std::random_device randomSeed;
	std::mt19937 mt(randomSeed());
	std::uniform_int_distribution<int> randomNumber(min, max);
	return randomNumber(mt);
}

int World::getPlayerHealth()
{
	if (m_player != nullptr)
		return m_player->getHealth();
}

float World::getPlayerEnergy()
{
	if (m_player != nullptr)
		return m_player->getSprintEnergyWithDecimals();
}