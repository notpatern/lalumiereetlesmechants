#include "GameManager.h"

#include "../Gameplay/Collision.h"

GameManager::GameManager() : m_missilesPool{ INITIAL_MISSILES_POOL_SIZE, MAX_MISSILES_POOL_SIZE }, m_enemiesPool{ INITIAL_ENEMIES_POOL_SIZE, MAX_ENEMIES_POOL_SIZE }
{
	SpawnTestEnemies();
}

void GameManager::Update(float deltaTime)
{
	m_missilesPool.Update(deltaTime);
	m_enemiesPool.Update(deltaTime);

	CheckMissilesHits();
}

void GameManager::SpawnTestEnemies()
{
	if (Enemy* enemy = m_enemiesPool.Acquire())
	{
		enemy->Spawn({ 30, 5}, {0,0});
	}
	if (Enemy* enemy = m_enemiesPool.Acquire())
	{
		enemy->Spawn({ 40, 5}, {0,0});
	}
	if (Enemy* enemy = m_enemiesPool.Acquire())
	{
		enemy->Spawn({ 50, 5}, {0,0});
	}
}

void GameManager::CheckMissilesHits()
{
	for (Missile& missile : m_missilesPool.getObjects())
	{
		if (!missile.getIsActive() || missile.getTeam() == Team::Enemy)
		{
			continue;
		}
		for (Enemy& enemy : m_enemiesPool.getObjects())
		{
			if (!enemy.getIsActive())
			{
				continue;
			}
			if (Collision::IsSameCell(missile.getPosition(), enemy.getPosition()))
			{
				missile.OnHit(enemy);
				break;
			}
		}
	}
}
