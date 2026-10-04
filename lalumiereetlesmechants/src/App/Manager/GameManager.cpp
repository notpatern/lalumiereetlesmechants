#include "GameManager.h"
#include "../Gameplay/Collision.h"
#include "../Gameplay/WaveData.h"

GameManager::GameManager() :
	m_missilesPool{INITIAL_MISSILES_POOL_SIZE, MAX_MISSILES_POOL_SIZE},
	m_enemiesPool{INITIAL_ENEMIES_POOL_SIZE, MAX_ENEMIES_POOL_SIZE},
	m_player{Utility::Vector2<float>(PLAYER_START_X, PLAYER_START_Y), m_missilesPool},
	m_waveSpawner{WaveData::GetWaves(), m_enemiesPool}
{
}

void GameManager::Update(float deltaTime, const InputManager& inputManager)
{
	if (deltaTime > MAX_DELTA_TIME)
	{
		deltaTime = MAX_DELTA_TIME;
	}

	m_player.Update(deltaTime, inputManager, PlayArea::Bounds());
	m_waveSpawner.Update(deltaTime);
	m_missilesPool.Update(deltaTime);
	m_enemiesPool.Update(deltaTime);

	CheckMissilesHits(m_enemiesPool, Team::Enemy);
	CheckPlayerContact();
	CheckPlayAreaExits();
}

void GameManager::CheckPlayAreaExits()
{
	const Collision::CellRect area = PlayArea::ExitBounds();

	CheckExits(m_enemiesPool, area);
	CheckExits(m_missilesPool, area);
}

void GameManager::CheckPlayerContact()
{
	if (m_player.IsDead())
	{
		return;
	}

	const Collision::CellRect playerRect = m_player.getHitboxRect();

	m_enemiesPool.ForEachActive([&](Enemy& enemy)
	{
		if (playerRect.Overlaps(enemy.getHitboxRect()))
		{
			m_player.TakeDamage(enemy.getContactDamage());
			enemy.TakeDamage(m_player.getContactDamage());
		}
	});
}

template <typename T>
void GameManager::CheckExits(Utility::Pool<T>& pool, const Collision::CellRect& area)
{
	pool.ForEachActive([&](T& object)
	{
		if (!area.Contains(Collision::ToCell(object.getCurrentPosition())))
		{
			object.OnLeftPlayArea(); //TODO : Peut etre un concept
		}
	});
}

template <Hittable Target>
void GameManager::CheckMissilesHits(Utility::Pool<Target>& targets, Team targetTeam)
{
	m_missilesPool.ForEachActive([&](Missile& missile)
	{
		if (missile.getTeam() == targetTeam)
		{
			return;
		}

		targets.ForEachActive([&](Target& target)
		{
			if (!missile.getIsActive())
			{
				return;
			}

			if (Collision::CrossesRect(missile.getLastPosition(), missile.getCurrentPosition(), target.getHitboxRect()))
			{
				missile.OnHit(target);
			}
		});
	});
}
