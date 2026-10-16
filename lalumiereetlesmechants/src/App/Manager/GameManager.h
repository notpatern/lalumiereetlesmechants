#pragma once
#include "InputManager.h"
#include "../Entites/Enemy.h"
#include "../Entites/Missile.h"
#include "../Entites/Particle.h"
#include "../Entites/XpOrb.h"
#include "../Gameplay/Collision.h"
#include "../Gameplay/DamageInfos.h"
#include "../Gameplay/Hittable.h"
#include "../Gameplay/ImpactFeedback.h"
#include "../Gameplay/PlayArea.h"
#include "../Gameplay/WaveSpawner.h"
#include "../Player/Player.h"
#include "../Render/SpriteType.h"
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Pool.h"
#include "../../Utilities/ScreenShake.h"
#include "../../Utilities/Vector2.h"
#include <cstddef>

class GameManager
{
public:
	GameManager();

	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	void Update(float inDeltaTime, const InputManager& inInputManager);

	void SkipWave()
	{
		m_waveSpawner.SkipToNextWave();
	}

	const Player& getPlayer() const { return m_player; }

	const Utility::Pool<Enemy>& getEnemies() const { return m_enemiesPool; }
	const Utility::Pool<Missile>& getPlayerMissiles() const { return m_playerMissilesPool; }
	const Utility::Pool<Missile>& getEnemyMissiles() const { return m_enemyMissilesPool; }
	const Utility::Pool<XpOrb>& getXpOrbs() const { return m_xpOrbsPool; }
	const Utility::Pool<Particle>& getParticles() const { return m_particlesPool; }

	int getScore() const { return m_score; }
	int getWaveNumber() const { return m_waveSpawner.getWaveNumber(); }
	int getWaveCount() const { return m_waveSpawner.getWaveCount(); }
	bool IsGameOver() const { return m_player.IsDead(); }
	bool IsVictory() const { return m_waveSpawner.IsFinished() && !m_player.IsDead(); }

	Utility::Vector2<int> getShakeOffset() const { return m_shake.getOffset(); }

private:
	static constexpr float MAX_DELTA_TIME = 0.1f;

	static constexpr std::size_t INITIAL_PLAYER_MISSILES_POOL_SIZE = 10;
	static constexpr std::size_t MAX_PLAYER_MISSILES_POOL_SIZE = 50;
	static constexpr std::size_t INITIAL_ENEMY_MISSILES_POOL_SIZE = 6;
	static constexpr std::size_t MAX_ENEMY_MISSILES_POOL_SIZE = 200;
	static constexpr std::size_t INITIAL_ENEMIES_POOL_SIZE = 6;
	static constexpr std::size_t MAX_ENEMIES_POOL_SIZE = 200;
	static constexpr std::size_t INITIAL_XP_ORBS_POOL_SIZE = 6;
	static constexpr std::size_t MAX_XP_ORBS_POOL_SIZE = 100;
	static constexpr std::size_t INITIAL_PARTICLES_POOL_SIZE = 64;
	static constexpr std::size_t MAX_PARTICLES_POOL_SIZE = 300;

	static constexpr int EXPLOSION_PARTICLE_COUNT = 8;
	static constexpr int HIT_PARTICLE_COUNT = 4;
	static constexpr int PLAYER_EXPLOSION_PARTICLE_COUNT = 16;
	static constexpr int LEVEL_UP_PARTICLE_COUNT = 12;
	static constexpr float XP_ORB_SPREAD = 1.f;
	static constexpr int XP_ORB_VALUE = 1;

	static constexpr ImpactFeedback PLAYER_HIT_FEEDBACK{ .hitStop = 0.12f, .shakeX = 2, .shakeY = 1, .shakeDuration = 0.35f };
	static constexpr ImpactFeedback PLAYER_KILLED_FEEDBACK{ .shakeX = 3, .shakeY = 2, .shakeDuration = 0.8f };

	static constexpr float PLAYER_START_X = PlayArea::WIDTH / 2.f;
	static constexpr float PLAYER_START_Y = PlayArea::HEIGHT / 2.f;

	Utility::Pool<Missile> m_playerMissilesPool;
	Utility::Pool<Missile> m_enemyMissilesPool;
	Utility::Pool<Enemy> m_enemiesPool;
	Utility::Pool<XpOrb> m_xpOrbsPool;
	Utility::Pool<Particle> m_particlesPool;

	Player m_player;
	WaveSpawner m_waveSpawner;

	int m_score = 0;

	Utility::Countdown m_hitStop;
	Utility::ScreenShake m_shake;

	void SeparateEnemies(float inDeltaTime);

	void CheckMissilesHitEnemies(Utility::Pool<Enemy>& outEnemies);
	void CheckMissilesHitPlayer();
	template <Hittable Target>
	static DamageResult TryHit(Missile& outMissile, Target& outTarget);

	void CheckPlayerContact();
	void CollectXpOrbs();

	void CheckPlayAreaExits();
	template <typename T>
	void CheckExits(Utility::Pool<T>& outPool, const Collision::CellRect& inArea);

	void OnEnemyKilled(const Enemy& inEnemy);
	void OnPlayerDamaged(DamageResult inResult, const Utility::Vector2<float>& inCenter);

	void PlayFeedback(const ImpactFeedback& inFeedback);
	void DropXpOrbs(const Utility::Vector2<float>& inCenter, int inCount);
	void SpawnExplosion(const Utility::Vector2<float>& inCenter, int inParticleCount, SpriteUtils::SpriteType inSpark = SpriteUtils::SpriteType::Spark, SpriteUtils::SpriteType inSmoke = SpriteUtils::SpriteType::Smoke);
};
