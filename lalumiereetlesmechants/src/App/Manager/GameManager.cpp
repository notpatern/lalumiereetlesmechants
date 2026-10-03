#include "GameManager.h"
#include "../Gameplay/Collision.h"
#include "../Gameplay/WaveData.h"

GameManager::GameManager() :
	m_missilesPool{ INITIAL_MISSILES_POOL_SIZE, MAX_MISSILES_POOL_SIZE },
	m_enemiesPool{ INITIAL_ENEMIES_POOL_SIZE, MAX_ENEMIES_POOL_SIZE },
	m_player{ Utility::Vector2<float>(PLAYER_START_X, PLAYER_START_Y), m_missilesPool },
	m_waveSpawner{ WaveData::GetWaves(), m_enemiesPool }
{
}
void GameManager::Update(float deltaTime, const InputManager& inputManager)
{
	if (deltaTime > MAX_DELTA_TIME)
	{
		deltaTime = MAX_DELTA_TIME;
	}

	m_player.Update(deltaTime, inputManager);
	m_waveSpawner.Update(deltaTime);
	m_missilesPool.Update(deltaTime);
	m_enemiesPool.Update(deltaTime);

	CheckMissilesHits(m_enemiesPool, Team::Enemy);
	DeactivateOutOfArea();
}

void GameManager::DeactivateOutOfArea()
{
	const Collision::CellRect area{
		{-OFFSCREEN_MARGIN, -OFFSCREEN_MARGIN},
	{PLAY_AREA_WIDTH - 1 + OFFSCREEN_MARGIN, PLAY_AREA_HEIGHT - 1 + OFFSCREEN_MARGIN}
	};

	DeactivateOutside(m_enemiesPool, area);
	DeactivateOutside(m_missilesPool, area);
}

template <typename T>
void GameManager::DeactivateOutside(Utility::Pool<T>& pool, const Collision::CellRect& area)
{
	for (T& object : pool.getObjects())
	{
		if (object.getIsActive() && !area.Contains(Collision::ToCell(object.getCurrentPosition())))
		{
			object.Deactivate();
		}
	}
}

template <Hittable Target>
void GameManager::CheckMissilesHits(Utility::Pool<Target>& targets, Team targetTeam)
{
	for (Missile& missile : m_missilesPool.getObjects())
	{
		if (!missile.getIsActive() || missile.getTeam() == targetTeam)
		{
			continue;
		}

		for (Target& target : targets.getObjects())
		{
			if (!target.getIsActive())
			{
				continue;
			}

			if (Collision::CrossesRect(missile.getLastPosition(), missile.getCurrentPosition(), target.getHitboxRect()))
			{
				missile.OnHit(target);
				break;
			}
		}
	}
}
