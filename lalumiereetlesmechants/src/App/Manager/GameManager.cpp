#include "GameManager.h"
#include "../Gameplay/Collision.h"
#include "../Gameplay/WaveData.h"
#include "../../Utilities/ScreenDirection.h"
#include <algorithm>
#include <numbers>

GameManager::GameManager() :
	m_missilesPool{INITIAL_MISSILES_POOL_SIZE, MAX_MISSILES_POOL_SIZE},
	m_enemiesPool{INITIAL_ENEMIES_POOL_SIZE, MAX_ENEMIES_POOL_SIZE},
	m_xpOrbsPool{INITIAL_XP_ORBS_POOL_SIZE, MAX_XP_ORBS_POOL_SIZE},
	m_particlesPool{INITIAL_PARTICLES_POOL_SIZE, MAX_PARTICLES_POOL_SIZE},
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

	m_shake.Update(deltaTime);
	m_particlesPool.Update(deltaTime);

	if (IsGameOver() || IsVictory())
	{
		return;
	}

	m_hitStop.Update(deltaTime);
	if (!m_hitStop.IsFinished())
	{
		return;
	}

	m_player.Update(deltaTime, inputManager, PlayArea::Bounds());
	m_waveSpawner.Update(deltaTime);
	SeparateEnemies(deltaTime);

	const Utility::Vector2<float> playerCenter = m_player.getAimPoint();
	const EnemyUpdateContext enemyContext{ .missiles = m_missilesPool, .playerCenter = playerCenter };
	m_enemiesPool.ForEachActive([&](Enemy& enemy)
	{
		enemy.Update(deltaTime, enemyContext);
		if (enemy.HasChargeJustEnded())
		{
			PlayFeedback(enemy.getArchetype().chargeEndFeedback);
		}
	});
	m_missilesPool.Update(deltaTime);
	m_xpOrbsPool.ForEachActive([&](XpOrb& orb)
	{
		orb.Update(deltaTime, playerCenter);
	});

	CheckMissilesHits(m_enemiesPool);
	CheckMissilesHits(m_player);
	CheckPlayerContact();
	CollectXpOrbs();
	CheckPlayAreaExits();
}

void GameManager::SeparateEnemies(float deltaTime)
{
	m_enemiesPool.ForEachActive([&](Enemy& enemy)
	{
		const MovementSettings& settings = enemy.getMovementSettings();
		if (settings.separationRadius <= 0.f)
		{
			return;
		}

		const float radiusSquared = settings.separationRadius * settings.separationRadius;
		Utility::Vector2<float> push{ 0.f, 0.f };

		m_enemiesPool.ForEachActive([&](const Enemy& other)
		{
			if (&other == &enemy || other.getMovementSettings().separationRadius <= 0.f)
			{
				return;
			}

			const float distanceSquared = Utility::ScreenDistanceSquared(other.getPosition(), enemy.getPosition());
			if (distanceSquared >= radiusSquared)
			{
				return;
			}

			const Utility::Vector2<float> away = Utility::ScreenDirection(other.getPosition(), enemy.getPosition());
			const float strength = 1.f - distanceSquared / radiusSquared;
			push.x += away.x * strength;
			push.y += away.y * strength;
		});

		if (Utility::ScreenDistanceSquared({ 0.f, 0.f }, push) > 1.f)
		{
			push = Utility::ScreenDirection({ 0.f, 0.f }, push);
		}

		const float step = settings.separationSpeed * deltaTime;
		enemy.Nudge({ push.x * step, push.y * step });
	});
}

void GameManager::CheckPlayAreaExits()
{
	const Collision::CellRect area = PlayArea::ExitBounds();

	CheckExits(m_enemiesPool, area);
	CheckExits(m_missilesPool, area);
	CheckExits(m_xpOrbsPool, PlayArea::Bounds());
	CheckExits(m_particlesPool, PlayArea::Bounds());
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
			OnPlayerDamaged(m_player.TakeDamage(enemy.getContactDamage()), playerRect.getCenter());

			if (enemy.TakeDamage(m_player.getContactDamage()) == DamageResult::Killed)
			{
				SpawnExplosion(enemy.getHitboxRect().getCenter(), EXPLOSION_PARTICLE_COUNT);
				PlayFeedback(enemy.getArchetype().killFeedback);
			}
		}
	});
}

void GameManager::OnEnemyKilled(const Enemy& enemy)
{
	m_score += enemy.getPoints();

	const Utility::Vector2<float> center = enemy.getHitboxRect().getCenter();
	DropXpOrbs(center, enemy.getXp());
	SpawnExplosion(center, EXPLOSION_PARTICLE_COUNT);
	PlayFeedback(enemy.getArchetype().killFeedback);
}

void GameManager::OnPlayerDamaged(DamageResult result, const Utility::Vector2<float>& center)
{
	if (result == DamageResult::Ignored)
	{
		return;
	}

	if (result == DamageResult::Killed)
	{
		SpawnExplosion(center, PLAYER_EXPLOSION_PARTICLE_COUNT);
		PlayFeedback(PLAYER_KILLED_FEEDBACK);
		return;
	}

	SpawnExplosion(center, HIT_PARTICLE_COUNT);
	PlayFeedback(PLAYER_HIT_FEEDBACK);
}

void GameManager::PlayFeedback(const ImpactFeedback& feedback)
{
	m_hitStop.StartIfLonger(feedback.hitStop);
	m_shake.Start({ feedback.shakeX, feedback.shakeY }, feedback.shakeDuration);
}

void GameManager::DropXpOrbs(const Utility::Vector2<float>& center, int count)
{
	const float angleStep = 2.f * std::numbers::pi_v<float> / count;

	for (int i = 0; i < count; ++i)
	{
		XpOrb* orb = m_xpOrbsPool.Acquire();
		if (!orb)
		{
			return;
		}

		const Utility::Vector2<float> offset = count > 1 ? Utility::ScreenVector(i * angleStep, XP_ORB_SPREAD) : Utility::Vector2<float>{ 0.f, 0.f };
		const float x = std::clamp(center.x + offset.x, 0.f, PlayArea::WIDTH - 1.f);
		const float y = std::clamp(center.y + offset.y, 0.f, PlayArea::HEIGHT - 1.f);
		orb->Spawn({ x, y }, 1);
	}
}

void GameManager::CollectXpOrbs()
{
	const Collision::CellRect playerBody = m_player.getBodyRect();
	const int levelBefore = m_player.getExperience().getLevel();

	m_xpOrbsPool.ForEachActive([&](XpOrb& orb)
	{
		if (playerBody.Contains(Collision::ToCell(orb.getPosition())))
		{
			m_player.AddXp(orb.getValue());
			orb.Collect();
		}
	});

	if (m_player.getExperience().getLevel() > levelBefore)
	{
		SpawnExplosion(m_player.getAimPoint(), LEVEL_UP_PARTICLE_COUNT, SpriteUtils::SpriteType::LevelUpSpark, SpriteUtils::SpriteType::LevelUpSpark);
	}
}

void GameManager::SpawnExplosion(const Utility::Vector2<float>& center, int particleCount, SpriteUtils::SpriteType spark, SpriteUtils::SpriteType smoke)
{
	const float angleStep = 2.f * std::numbers::pi_v<float> / particleCount;

	for (int i = 0; i < particleCount; ++i)
	{
		if (Particle* particle = m_particlesPool.Acquire())
		{
			particle->Spawn(center, i * angleStep, spark, smoke);
		}
	}
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
				OnEnemyKilled(target);
			}
		});
	});
}

template <Hittable Target>
void GameManager::CheckMissilesHits(Target& target)
{
	m_missilesPool.ForEachActive([&](Missile& missile)
	{
		OnPlayerDamaged(TryHit(missile, target), target.getHitboxRect().getCenter());
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
