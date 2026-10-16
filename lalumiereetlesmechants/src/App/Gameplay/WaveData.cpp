#include "WaveData.h"

#include "EnemyArchetype.h"
#include "PlayArea.h"
#include "../../Utilities/Vector2.h"

namespace WaveData
{
	namespace
	{
		constexpr float TURRET_SPEED = 8.f;
		constexpr float TURRET_LEFT_X = -5.f;
		constexpr float TURRET_RIGHT_X = static_cast<float>(PlayArea::WIDTH);
		constexpr float CHARGER_STOP_ROW = 6.f;
		constexpr float SWARM_INTERVAL = 0.2f;

		static_assert(TURRET_LEFT_X >= -PlayArea::OFFSCREEN_MARGIN && TURRET_RIGHT_X <= PlayArea::WIDTH - 1 + PlayArea::OFFSCREEN_MARGIN, "Les tourelles doivent apparaitre dans la marge, sinon elles sont desactivees des leur apparition!!!");

		SpawnEntry Hunter(float delay, float x, const Utility::Vector2<float>& target)
		{
			return { .delay = delay, .type = EnemyType::Hunter, .position = { x, PlayArea::SPAWN_Y }, .target = target };
		}

		SpawnEntry TurretFromLeft(float delay, float row)
		{
			return { .delay = delay, .type = EnemyType::Turret, .position = { TURRET_LEFT_X, row }, .velocity = { TURRET_SPEED, 0.f } };
		}

		SpawnEntry TurretFromRight(float delay, float row)
		{
			return { .delay = delay, .type = EnemyType::Turret, .position = { TURRET_RIGHT_X, row }, .velocity = { -TURRET_SPEED, 0.f } };
		}

		SpawnEntry Charger(float delay, float x)
		{
			return { .delay = delay, .type = EnemyType::Charger, .position = { x, PlayArea::SPAWN_Y }, .target = { x, CHARGER_STOP_ROW } };
		}

		SpawnEntry Swarm(float delay, float x, int count)
		{
			return { .delay = delay, .type = EnemyType::Swarm, .position = { x, PlayArea::SPAWN_Y }, .target = { x, static_cast<float>(PlayArea::HEIGHT) }, .count = count, .interval = SWARM_INTERVAL };
		}
	}

	const std::vector<Wave>& GetWaves()
	{
		static const std::vector<Wave> waves =
		{
			{
				.spawnEntries =
				{
					Hunter(0.f, 10.f, { 50.f, 20.f }),
					Hunter(1.f, 40.f, { 40.f, 20.f }),
					Hunter(1.f, 70.f, { 30.f, 20.f }),
				},
				.pauseAfterWave = 2.f,
			},
			{
				.spawnEntries =
				{
					Hunter(0.f, 5.f, { 60.f, 22.f }), Hunter(0.f, 75.f, { 20.f, 22.f }),
					Hunter(1.5f, 20.f, { 55.f, 18.f }), Hunter(0.f, 60.f, { 25.f, 18.f }),
					Charger(1.5f, 38.f),
				},
				.pauseAfterWave = 2.f,
			},
			{
				.spawnEntries =
				{
					TurretFromLeft(0.f, 3.f),
					Hunter(3.f, 40.f, { 40.f, 22.f }),
					Swarm(1.f, 15.f, 6),
				},
				.pauseAfterWave = 2.f,
			},
			{
				.spawnEntries =
				{
					Hunter(0.f, 40.f, { 40.f, 24.f }),
					Hunter(0.5f, 30.f, { 20.f, 24.f }), Hunter(0.f, 50.f, { 60.f, 24.f }),
					Hunter(0.5f, 20.f, { 5.f, 24.f }), Hunter(0.f, 60.f, { 75.f, 24.f }),
				},
				.pauseAfterWave = 2.f,
			},
			{
				.spawnEntries =
				{
					TurretFromLeft(0.f, 2.f), TurretFromRight(0.f, 4.f),
					Swarm(2.f, 60.f, 8),
				},
				.pauseAfterWave = 3.f,
			},
			{
				.spawnEntries =
				{
					Hunter(0.f, 5.f, { 45.f, 22.f }),
					Hunter(0.6f, 75.f, { 35.f, 22.f }),
					Hunter(0.6f, 15.f, { 55.f, 22.f }),
					Hunter(0.6f, 65.f, { 25.f, 22.f }),
					Hunter(0.6f, 25.f, { 65.f, 22.f }),
					Hunter(0.6f, 55.f, { 15.f, 22.f }),
					Charger(1.f, 20.f),
				},
				.pauseAfterWave = 2.f,
			},
			{
				.spawnEntries =
				{
					TurretFromRight(0.f, 3.f),
					Hunter(2.f, 10.f, { 40.f, 22.f }), Hunter(0.f, 70.f, { 40.f, 22.f }),
					Hunter(1.5f, 30.f, { 70.f, 20.f }), Hunter(0.f, 50.f, { 10.f, 20.f }),
				},
				.pauseAfterWave = 2.f,
			},
			{
				.spawnEntries =
				{
					TurretFromLeft(0.f, 2.f),
					TurretFromRight(1.5f, 4.f),
					Hunter(1.5f, 40.f, { 20.f, 24.f }), Hunter(0.f, 40.f, { 60.f, 24.f }),
					Hunter(1.5f, 15.f, { 40.f, 22.f }), Hunter(0.f, 65.f, { 40.f, 22.f }),
				},
				.pauseAfterWave = 3.f,
			},
			{
				.spawnEntries =
				{
					Hunter(0.f, 2.f, { 50.f, 24.f }), Hunter(0.f, 77.f, { 30.f, 24.f }),
					Hunter(0.5f, 10.f, { 55.f, 22.f }), Hunter(0.f, 70.f, { 25.f, 22.f }),
					Hunter(0.5f, 18.f, { 60.f, 20.f }), Hunter(0.f, 62.f, { 20.f, 20.f }),
					Hunter(0.5f, 26.f, { 65.f, 18.f }), Hunter(0.f, 54.f, { 15.f, 18.f }),
				},
				.pauseAfterWave = 3.f,
			},
			{
				.spawnEntries =
				{
					TurretFromLeft(0.f, 2.f),
					TurretFromRight(1.f, 3.f),
					TurretFromLeft(1.f, 4.f),
					Hunter(2.f, 10.f, { 40.f, 24.f }), Hunter(0.f, 70.f, { 40.f, 24.f }),
					Hunter(1.f, 25.f, { 60.f, 22.f }), Hunter(0.f, 55.f, { 20.f, 22.f }),
					Hunter(1.f, 40.f, { 40.f, 24.f }),
				},
				.pauseAfterWave = 3.f,
			},
		};

		return waves;
	}
}
