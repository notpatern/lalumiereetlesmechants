#pragma once
#include "InputManager.h"
#include "../Entites/Missile.h"
#include "../../Utilities/Pool.h"
#include "../Entites/Enemy.h"
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

	Utility::Pool<Missile> m_missilesPool;
	Utility::Pool<Enemy> m_enemiesPool;

	void Update(float deltaTime, const InputManager& inputManager);

	const Player& getPlayer() const { return m_player; }

private:
	static constexpr float MAX_DELTA_TIME = 0.1f;

	static constexpr std::size_t INITIAL_MISSILES_POOL_SIZE = 6;
	static constexpr std::size_t MAX_MISSILES_POOL_SIZE = 200;
	static constexpr std::size_t INITIAL_ENEMIES_POOL_SIZE = 6;
	static constexpr std::size_t MAX_ENEMIES_POOL_SIZE = 200;

	static constexpr float PLAYER_START_X = PlayArea::WIDTH / 2.f;
	static constexpr float PLAYER_START_Y = PlayArea::HEIGHT / 2.f;

	Player m_player;
	WaveSpawner m_waveSpawner;

	void CheckPlayAreaExits();

	template <typename T>
	void CheckExits(Utility::Pool<T>& pool, const Collision::CellRect& area);

	template <Hittable Target>
	void CheckMissilesHits(Utility::Pool<Target>& targets);

	template <Hittable Target>
	void CheckMissilesHits(Target& target);

	template <Hittable Target>
	static void TryHit(Missile& missile, Target& target);

	void CheckPlayerContact();
};
