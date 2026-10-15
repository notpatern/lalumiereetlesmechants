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
	if (IsGameOver() || IsVictory())
	{
		return;
	}

	if (deltaTime > MAX_DELTA_TIME)
	{
		deltaTime = MAX_DELTA_TIME;
	}

	m_player.Update(deltaTime, inputManager, PlayArea::Bounds());
	m_waveSpawner.Update(deltaTime);

	const EnemyUpdateContext enemyContext{ .missiles = m_missilesPool, .playerCenter = m_player.getAimPoint() };
	m_enemiesPool.ForEachActive([&](Enemy& enemy)
	{
		enemy.Update(deltaTime, enemyContext);
	});
	m_missilesPool.Update(deltaTime);

	CheckMissilesHits(m_enemiesPool);
	CheckMissilesHits(m_player);
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
		if (!area.Contains(Collision::ToCell(object.getPosition())))
		{
			object.OnLeftPlayArea(); //TODO : Peut etre un concept
		}
	});
}

template <Hittable Target>
void GameManager::CheckMissilesHits(Utility::Pool<Target>& targets)
{
	m_missilesPool.ForEachActive([&](Missile& missile)
	{
		targets.ForEachActive([&](Target& target)
		{
			if (TryHit(missile, target) == DamageResult::Killed)
			{
				m_score += target.getPoints();
			}
		});
	});
}

template <Hittable Target>
void GameManager::CheckMissilesHits(Target& target)
{
	m_missilesPool.ForEachActive([&](Missile& missile)
	{
		TryHit(missile, target);
	});
}

template <Hittable Target>
DamageResult GameManager::TryHit(Missile& missile, Target& target)
{
	if (!missile.getIsActive() || !target.getIsActive() || missile.getTeam() == target.getTeam())
	{
		return DamageResult::Ignored;
	}

	const Collision::CellRect targetRect = target.getHitboxRect();

	if (!PlayArea::Bounds().Contains(targetRect))
	{
		return DamageResult::Ignored;
	}

	Collision::CellRect hitArea = targetRect;
	hitArea.topLeftCell.x -= missile.getHitboxSize().x - 1;
	hitArea.topLeftCell.y -= missile.getHitboxSize().y - 1;

	if (!Collision::CrossesRect(missile.getLastPosition(), missile.getPosition(), hitArea))
	{
		return DamageResult::Ignored;
	}

	return missile.OnHit(target);
}
