#include "GameManager.h"
#include "../Gameplay/Collision.h"
#include "../Gameplay/WaveData.h"
#include "../../Utilities/ScreenDirection.h"
#include <algorithm>
#include <numbers>

GameManager::GameManager()
	: m_missilesPool{ INITIAL_MISSILES_POOL_SIZE, MAX_MISSILES_POOL_SIZE },
	m_enemiesPool{ INITIAL_ENEMIES_POOL_SIZE, MAX_ENEMIES_POOL_SIZE },
	m_xpOrbsPool{ INITIAL_XP_ORBS_POOL_SIZE, MAX_XP_ORBS_POOL_SIZE },
	m_particlesPool{ INITIAL_PARTICLES_POOL_SIZE, MAX_PARTICLES_POOL_SIZE },
	m_player{ Utility::Vector2<float>(PLAYER_START_X, PLAYER_START_Y), m_missilesPool },
	m_waveSpawner{ WaveData::GetWaves(), m_enemiesPool }
{
}

void GameManager::Update(float inDeltaTime, const InputManager& inInputManager)
{
	const float deltaTime = std::min(inDeltaTime, MAX_DELTA_TIME);

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

	m_player.Update(deltaTime, inInputManager, PlayArea::Bounds());
	m_waveSpawner.Update(deltaTime);
	SeparateEnemies(deltaTime);

	const Utility::Vector2<float> playerCenter = m_player.getAimPoint();
	const EnemyUpdateContext enemyContext{ .missiles = m_missilesPool, .playerCenter = playerCenter };
	m_enemiesPool.ForEachActive([&](Enemy& outEnemy)
	{
		outEnemy.Update(deltaTime, enemyContext);
		if (outEnemy.HasChargeJustEnded())
		{
			PlayFeedback(outEnemy.getArchetype().chargeEndFeedback);
		}
	});
	m_missilesPool.Update(deltaTime);
	m_xpOrbsPool.ForEachActive([&](XpOrb& outOrb)
	{
		outOrb.Update(deltaTime, playerCenter);
	});

	CheckMissilesHitEnemies(m_enemiesPool);
	CheckMissilesHitPlayer();
	CheckPlayerContact();
	CollectXpOrbs();
	CheckPlayAreaExits();
}

void GameManager::SeparateEnemies(float inDeltaTime)
{
	m_enemiesPool.ForEachActive([&](Enemy& outEnemy)
	{
		const MovementSettings& settings = outEnemy.getMovementSettings();
		if (settings.separationRadius <= 0.f)
		{
			return;
		}

		const float radiusSquared = settings.separationRadius * settings.separationRadius;
		Utility::Vector2<float> push{ 0.f, 0.f };

		m_enemiesPool.ForEachActive([&](const Enemy& inOther)
		{
			if (&inOther == &outEnemy || inOther.getMovementSettings().separationRadius <= 0.f)
			{
				return;
			}

			const float distanceSquared = Utility::ScreenDistanceSquared(inOther.getPosition(), outEnemy.getPosition());
			if (distanceSquared >= radiusSquared)
			{
				return;
			}

			const Utility::Vector2<float> away = Utility::ScreenDirection(inOther.getPosition(), outEnemy.getPosition());
			const float strength = 1.f - distanceSquared / radiusSquared;
			push.x += away.x * strength;
			push.y += away.y * strength;
		});

		if (Utility::ScreenDistanceSquared({ 0.f, 0.f }, push) > 1.f)
		{
			push = Utility::ScreenDirection({ 0.f, 0.f }, push);
		}

		const float step = settings.separationSpeed * inDeltaTime;
		outEnemy.Nudge({ push.x * step, push.y * step });
	});
}

void GameManager::CheckMissilesHitEnemies(Utility::Pool<Enemy>& outEnemies)
{
	m_missilesPool.ForEachActive([&](Missile& outMissile)
	{
		outEnemies.ForEachActive([&](Enemy& outEnemy)
		{
			if (TryHit(outMissile, outEnemy) == DamageResult::Killed)
			{
				OnEnemyKilled(outEnemy);
			}
		});
	});
}

void GameManager::CheckMissilesHitPlayer()
{
	m_missilesPool.ForEachActive([&](Missile& outMissile)
	{
		OnPlayerDamaged(TryHit(outMissile, m_player), m_player.getHitboxRect().getCenter());
	});
}

template <Hittable Target>
DamageResult GameManager::TryHit(Missile& outMissile, Target& outTarget)
{
	if (!outMissile.IsActive() || !outTarget.IsActive() || outMissile.getTeam() == outTarget.getTeam())
	{
		return DamageResult::Ignored;
	}

	const Collision::CellRect targetRect = outTarget.getHitboxRect();

	if (!PlayArea::Bounds().ContainsRect(targetRect))
	{
		return DamageResult::Ignored;
	}

	Collision::CellRect hitArea = targetRect;
	hitArea.topLeftCell.x -= outMissile.getHitboxSize().x - 1;
	hitArea.topLeftCell.y -= outMissile.getHitboxSize().y - 1;

	if (!Collision::CrossesRect(outMissile.getLastPosition(), outMissile.getPosition(), hitArea))
	{
		return DamageResult::Ignored;
	}

	return outMissile.OnHit(outTarget);
}

void GameManager::CheckPlayerContact()
{
	if (m_player.IsDead())
	{
		return;
	}

	const Collision::CellRect playerRect = m_player.getHitboxRect();

	m_enemiesPool.ForEachActive([&](Enemy& outEnemy)
	{
		if (playerRect.Overlaps(outEnemy.getHitboxRect()))
		{
			OnPlayerDamaged(m_player.TakeDamage(outEnemy.getContactDamage()), playerRect.getCenter());

			if (outEnemy.TakeDamage(m_player.getContactDamage()) == DamageResult::Killed)
			{
				SpawnExplosion(outEnemy.getHitboxRect().getCenter(), EXPLOSION_PARTICLE_COUNT);
				PlayFeedback(outEnemy.getArchetype().killFeedback);
			}
		}
	});
}

void GameManager::CollectXpOrbs()
{
	const Collision::CellRect playerBody = m_player.getBodyRect();
	const int levelBefore = m_player.getExperience().getLevel();

	m_xpOrbsPool.ForEachActive([&](XpOrb& outOrb)
	{
		if (playerBody.ContainsCell(Collision::ToCell(outOrb.getPosition())))
		{
			m_player.AddXp(outOrb.getValue());
			outOrb.Collect();
		}
	});

	if (m_player.getExperience().getLevel() > levelBefore)
	{
		SpawnExplosion(m_player.getAimPoint(), LEVEL_UP_PARTICLE_COUNT, SpriteUtils::SpriteType::LevelUpSpark, SpriteUtils::SpriteType::LevelUpSpark);
	}
}

void GameManager::CheckPlayAreaExits()
{
	const Collision::CellRect area = PlayArea::ExitBounds();

	CheckExits(m_enemiesPool, area);
	CheckExits(m_missilesPool, area);
	CheckExits(m_xpOrbsPool, PlayArea::Bounds());
	CheckExits(m_particlesPool, PlayArea::Bounds());
}

template <typename T>
void GameManager::CheckExits(Utility::Pool<T>& outPool, const Collision::CellRect& inArea)
{
	outPool.ForEachActive([&](T& outObject)
	{
		if (!inArea.ContainsCell(Collision::ToCell(outObject.getPosition())))
		{
			outObject.OnLeftPlayArea();
		}
	});
}

void GameManager::OnEnemyKilled(const Enemy& inEnemy)
{
	m_score += inEnemy.getPoints();

	const Utility::Vector2<float> center = inEnemy.getHitboxRect().getCenter();
	DropXpOrbs(center, inEnemy.getXp());
	SpawnExplosion(center, EXPLOSION_PARTICLE_COUNT);
	PlayFeedback(inEnemy.getArchetype().killFeedback);
}

void GameManager::OnPlayerDamaged(DamageResult inResult, const Utility::Vector2<float>& inCenter)
{
	if (inResult == DamageResult::Ignored)
	{
		return;
	}

	if (inResult == DamageResult::Killed)
	{
		SpawnExplosion(inCenter, PLAYER_EXPLOSION_PARTICLE_COUNT);
		PlayFeedback(PLAYER_KILLED_FEEDBACK);
		return;
	}

	SpawnExplosion(inCenter, HIT_PARTICLE_COUNT);
	PlayFeedback(PLAYER_HIT_FEEDBACK);
}

void GameManager::PlayFeedback(const ImpactFeedback& inFeedback)
{
	m_hitStop.StartIfLonger(inFeedback.hitStop);
	m_shake.Start({ inFeedback.shakeX, inFeedback.shakeY }, inFeedback.shakeDuration);
}

void GameManager::DropXpOrbs(const Utility::Vector2<float>& inCenter, int inCount)
{
	const float angleStep = 2.f * std::numbers::pi_v<float> / inCount;

	for (int i = 0; i < inCount; ++i)
	{
		XpOrb* orb = m_xpOrbsPool.Acquire();
		if (!orb)
		{
			return;
		}

		const Utility::Vector2<float> offset = inCount > 1 ? Utility::ScreenVector(i * angleStep, XP_ORB_SPREAD) : Utility::Vector2<float>{ 0.f, 0.f };
		const float x = std::clamp(inCenter.x + offset.x, 0.f, PlayArea::WIDTH - 1.f);
		const float y = std::clamp(inCenter.y + offset.y, 0.f, PlayArea::HEIGHT - 1.f);
		orb->Spawn({ x, y }, XP_ORB_VALUE);
	}
}

void GameManager::SpawnExplosion(const Utility::Vector2<float>& inCenter, int inParticleCount, SpriteUtils::SpriteType inSpark, SpriteUtils::SpriteType inSmoke)
{
	const float angleStep = 2.f * std::numbers::pi_v<float> / inParticleCount;

	for (int i = 0; i < inParticleCount; ++i)
	{
		if (Particle* particle = m_particlesPool.Acquire())
		{
			particle->Spawn(inCenter, i * angleStep, inSpark, inSmoke);
		}
	}
}
