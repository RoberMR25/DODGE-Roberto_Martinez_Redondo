#pragma once

#include <cstdint>
#include <iostream>
#include <list>
#include <vector>

#include <Gameplay/PowerUps.h>
#include <Gameplay/SlimeBlueEasy.h>
#include <Gameplay/SlimeGreenMedium.h>
#include <Gameplay/SlimeRedHard.h>

#include <Utils/ObjectPool.h>

namespace sf
{
	class RenderWindow;
}

namespace tmx
{
	class Map;
}

class AudioManager;
class Enemy;
class MapLayer;
class ObjectLayer;
class Player;
class PowerUps;

class World
{
	public:

		World();
		~World();

		bool load();

		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

		void checkCollision();
		void checkCollisionForEnemies();
		void chooseHealthLost(int playerHealthLost);

		int getScore() const { return m_playerScore; }
		void setScoreByTimer() { m_playerScore += m_scoreAddedPerSecond; }
		void setScoreByCoin(int scoreAdded);
		int getCoinsCollected() { return m_coinsCollected; }
		int getPlayerHealth();
		float getPlayerEnergy();

		void updateTimer(uint32_t deltaMillisecondsForTimer);

		void enemySpawn();
		void slimeBlueEasySpawn();
		void slimeGreenMediumSpawn();
		void slimeRedHardSpawn();
		sf::Vector2f setEnemySpawnPosition(sf::Vector2f enemyDirection);

		float floatRandomRange(float min, float max);
		int intRandomRange(int min, int max);

		void powerUpsSpawn();
		void coinsSpawn();
		void heartsSpawn();
		void energySpawn();
		void checkPowerUpsForDespawn();

	private:

		float m_millisecondsToSeconds{ 0.f };
		float m_time{ 0.f };

		ObjectPool<SlimeBlueEasy, 50> m_slimeBlueEasyObjectPool;
		ObjectPool<SlimeGreenMedium, 50> m_slimeGreenMediumObjectPool;
		ObjectPool<SlimeRedHard, 50> m_slimeRedHardObjectPool;

		std::vector<SlimeBlueEasy*> m_slimeBlueEasyEnemies;
		std::vector<SlimeGreenMedium*> m_slimeGreenMediumEnemies;
		std::vector<SlimeRedHard*> m_slimeRedHardEnemies;

		Player* m_player{ nullptr };

		ObjectPool<PowerUps, 10> m_powerUpsObjectPool;
		std::vector<PowerUps*> m_powerUpsVector;
		int m_timeForPowerUpsToSpawn{ 5 };

		tmx::Map* m_map{ nullptr };
		MapLayer* m_layerZero{ nullptr };
		MapLayer* m_layerOne{ nullptr };
		MapLayer* m_layerTwo{ nullptr };
		ObjectLayer* m_groundCollisionLayer{ nullptr };
		ObjectLayer* m_leftWallCollisionLayer{ nullptr };
		ObjectLayer* m_rightWallCollisionLayer{ nullptr };
		ObjectLayer* m_topCollisionLayer{ nullptr };
		ObjectLayer* m_enemyDespawnCollisionLayer{ nullptr };

		float m_collisionOffset{ 0.f };
		bool m_hasInitiatedCollision{ false };
		bool m_hasInitiatedCollisionWithPowerUp{ false };

		int m_playerScore{ 0 };
		int m_scoreAddedPerSecond{ 0 };
		int m_previousSecond{ 0 };
		int m_coinsCollected{ 0 };
		
		float m_scoreRequiredToChangeFromEasyToMediumDifficulty{ 150.f };		// Values are low so that you can easily test it.
		float m_scoreRequiredToChangeFromMediumToHardDifficulty{ 300.f };		// For a "final edition" of the game, these values
																				// would be higher.
		// Enemies' Spawn Time depending of the difficulty
		float m_enemySpawnTime{ 0.0f };
		float m_enemySpawnTimeEasyDif{ 0.75f };
		float m_enemySpawnTimeMediumDif{ 0.5f };
		float m_enemySpawnTimeHardDif{ 0.25f };

		// Enemies' speeds depending of difficulty
		sf::Vector2f m_blueEnemySpeed{ 0.f, 0.f };
		sf::Vector2f m_greenEnemySpeed{ 0.f, 0.f };
		sf::Vector2f m_redEnemySpeed{ 0.f, 0.f };

		sf::Vector2f m_blueSlimeSpeed_EasyDif{ 120.f, 120.f };
		sf::Vector2f m_greenSlimeSpeed_EasyDif{ 80.f, 80.f };
		sf::Vector2f m_redSlimeSpeed_EasyDif{ 50.f, 50.f };

		sf::Vector2f m_blueSlimeSpeed_MediumDif{ 150.f, 150.f };
		sf::Vector2f m_greenSlimeSpeed_MediumDif{ 110.f, 110.f };
		sf::Vector2f m_redSlimeSpeed_MediumDif{ 75.f, 75.f };

		sf::Vector2f m_blueSlimeSpeed_HardDif{ 180.f, 180.f };
		sf::Vector2f m_greenSlimeSpeed_HardDif{ 140.f, 140.f };
		sf::Vector2f m_redSlimeSpeed_HardDif{ 100.f, 100.f };

		AudioManager* m_audioManager{ nullptr };
};