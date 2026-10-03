#pragma once
#include "InputManager.h"
#include "../Entites/Missile.h"
#include "../../Utilities/Pool.h"
#include "../Entites/Enemy.h"
#include "../Gameplay/Hittable.h"
#include "../Player/Player.h"


class GameManager
{
public:
	GameManager();

	Utility::Pool<Missile> m_missilesPool;
	Utility::Pool<Enemy> m_enemiesPool;

	void Update(float deltaTime, const InputManager& inputManager);

	const Player& getPlayer() const { return m_player; }

private:
	static constexpr float MAX_DELTA_TIME = 0.1f;
	static constexpr int PLAY_AREA_WIDTH = 80;
	static constexpr int PLAY_AREA_HEIGHT = 40;
	static constexpr int OFFSCREEN_MARGIN = 5;


	static constexpr std::size_t INITIAL_MISSILES_POOL_SIZE = 6;
	static constexpr std::size_t MAX_MISSILES_POOL_SIZE = 200;
	static constexpr std::size_t INITIAL_ENEMIES_POOL_SIZE = 6;
	static constexpr std::size_t MAX_ENEMIES_POOL_SIZE = 200;

	static constexpr float PLAYER_START_X = 40.f;
	static constexpr float PLAYER_START_Y = 20.f;

	Player m_player;

	void DeactivateOutOfArea();

	template <typename T>
	void DeactivateOutside(Utility::Pool<T>& pool, const Collision::CellRect& area);

	void SpawnTestEnemies();

	template <Hittable Target>
	void CheckMissilesHits(Utility::Pool<Target>& targets, Team targetTeam);
};
