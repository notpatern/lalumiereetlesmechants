#pragma once
#include "InputManager.h"
#include "../Entites/Missile.h"
#include "../../Utilities/Pool.h"
#include "../Entites/Enemy.h"
#include "../Entites/XpOrb.h"
#include "../Entites/Particle.h"
#include "../Gameplay/Hittable.h"
#include "../Gameplay/PlayArea.h"
#include "../Gameplay/WaveSpawner.h"
#include "../Player/Player.h"


class GameManager
{
public:
	GameManager();
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	void Update(float deltaTime, const InputManager& inputManager);

	const Player& getPlayer() const { return m_player; }

	const Utility::Pool<Enemy>& getEnemies() const { return m_enemiesPool; }
	const Utility::Pool<Missile>& getMissiles() const { return m_missilesPool; }
	const Utility::Pool<XpOrb>& getXpOrbs() const { return m_xpOrbsPool; }
	const Utility::Pool<Particle>& getParticles() const { return m_particlesPool; }

	int getScore() const { return m_score; }
	bool IsGameOver() const { return m_player.IsDead(); }
	bool IsVictory() const { return m_waveSpawner.IsFinished() && !m_player.IsDead(); }

private:
	static constexpr float MAX_DELTA_TIME = 0.1f;

	static constexpr std::size_t INITIAL_MISSILES_POOL_SIZE = 6;
	static constexpr std::size_t MAX_MISSILES_POOL_SIZE = 200;
	static constexpr std::size_t INITIAL_ENEMIES_POOL_SIZE = 6;
	static constexpr std::size_t MAX_ENEMIES_POOL_SIZE = 200;
	static constexpr std::size_t INITIAL_XP_ORBS_POOL_SIZE = 6;
	static constexpr std::size_t MAX_XP_ORBS_POOL_SIZE = 100;
	static constexpr std::size_t INITIAL_PARTICLES_POOL_SIZE = 64;
	static constexpr std::size_t MAX_PARTICLES_POOL_SIZE = 300;

	static constexpr int EXPLOSION_PARTICLE_COUNT = 8;
	static constexpr int HIT_PARTICLE_COUNT = 4;
	static constexpr float XP_ORB_SPREAD = 1.f;

	static constexpr float PLAYER_START_X = PlayArea::WIDTH / 2.f;
	static constexpr float PLAYER_START_Y = PlayArea::HEIGHT / 2.f;

	Utility::Pool<Missile> m_missilesPool;
	Utility::Pool<Enemy> m_enemiesPool;
	Utility::Pool<XpOrb> m_xpOrbsPool;
	Utility::Pool<Particle> m_particlesPool;

	Player m_player;
	WaveSpawner m_waveSpawner;

	int m_score = 0;

	void SeparateEnemies(float deltaTime);

	void CheckPlayAreaExits();

	template <typename T>
	void CheckExits(Utility::Pool<T>& pool, const Collision::CellRect& area);

	template <Hittable Target>
	void CheckMissilesHits(Utility::Pool<Target>& targets);

	template <Hittable Target>
	void CheckMissilesHits(Target& target);

	template <Hittable Target>
	static DamageResult TryHit(Missile& missile, Target& target);

	void CheckPlayerContact();

	void OnEnemyKilled(const Enemy& enemy);
	void DropXpOrbs(const Utility::Vector2<float>& center, int count);

	void CollectXpOrbs();
	void SpawnExplosion(const Utility::Vector2<float>& center, int particleCount);
};
