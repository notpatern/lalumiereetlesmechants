#pragma once
#include "../Entites/Missile.h"
#include "../../Utilities/Pool.h"
#include "../Entites/Enemy.h"
#include "../Gameplay/Hittable.h"


class GameManager
{
public:
	GameManager();

	Utility::Pool<Missile> m_missilesPool;
	Utility::Pool<Enemy> m_enemiesPool;

	void Update(float deltaTime);

private:
	static constexpr std::size_t INITIAL_MISSILES_POOL_SIZE = 6;
	static constexpr std::size_t MAX_MISSILES_POOL_SIZE = 200;
	static constexpr std::size_t INITIAL_ENEMIES_POOL_SIZE = 6;
	static constexpr std::size_t MAX_ENEMIES_POOL_SIZE = 200;

	void SpawnTestEnemies();

	template <Hittable Target>
	void CheckMissilesHits(Utility::Pool<Target>& targets, Team targetTeam);
};
