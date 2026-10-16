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
		constexpr float TURRET_HIGH_ROW = 2.f;
		constexpr float TURRET_LOW_ROW = 7.f;
		constexpr float SWARM_INTERVAL = 0.2f;
		constexpr float HUNTER_TRAIN_INTERVAL = 1.f;

		constexpr float OFFSCREEN_LEFT_X = -3.f;
		constexpr float OFFSCREEN_RIGHT_X = static_cast<float>(PlayArea::WIDTH);
		constexpr float LEFT_LANE_X = 12.f;
		constexpr float CENTER_X = 38.f;
		constexpr float RIGHT_LANE_X = 65.f;
		constexpr float BOTTOM_Y = static_cast<float>(PlayArea::HEIGHT);
		constexpr float WEST_ROW = 11.f;
		constexpr float EAST_ROW = 15.f;

		static_assert(TURRET_LEFT_X >= -PlayArea::OFFSCREEN_MARGIN && TURRET_RIGHT_X <= PlayArea::WIDTH - 1 + PlayArea::OFFSCREEN_MARGIN, "Les tourelles doivent apparaitre dans la marge, sinon elles sont desactivees des leur apparition!!!");
		static_assert(OFFSCREEN_LEFT_X >= -PlayArea::OFFSCREEN_MARGIN && OFFSCREEN_RIGHT_X <= PlayArea::WIDTH - 1 + PlayArea::OFFSCREEN_MARGIN, "Les trajets doivent commencer dans la marge, sinon l'ennemi est desactive des son apparition!!!");

		enum class Path
		{
			NorthToSouth,
			NorthWestToSouthEast,
			NorthEastToSouthWest,
			WestToEast,
			EastToWest
		};

		struct PathPoints
		{
			Utility::Vector2<float> start;
			Utility::Vector2<float> target;
		};

		PathPoints GetPathPoints(Path path)
		{
			switch (path)
			{
			case Path::NorthWestToSouthEast:
				return { { OFFSCREEN_LEFT_X, PlayArea::SPAWN_Y }, { OFFSCREEN_RIGHT_X, BOTTOM_Y } };
			case Path::NorthEastToSouthWest:
				return { { OFFSCREEN_RIGHT_X, PlayArea::SPAWN_Y }, { OFFSCREEN_LEFT_X, BOTTOM_Y } };
			case Path::WestToEast:
				return { { OFFSCREEN_LEFT_X, WEST_ROW }, { OFFSCREEN_RIGHT_X, WEST_ROW } };
			case Path::EastToWest:
				return { { OFFSCREEN_RIGHT_X, EAST_ROW }, { OFFSCREEN_LEFT_X, EAST_ROW } };
			case Path::NorthToSouth:
			default:
				return { { CENTER_X, PlayArea::SPAWN_Y }, { CENTER_X, BOTTOM_Y } };
			}
		}

		SpawnEntry Hunter(float delay, Path path)
		{
			const PathPoints points = GetPathPoints(path);
			return { .delay = delay, .type = EnemyType::Hunter, .position = points.start, .target = points.target };
		}

		SpawnEntry HunterTrain(float delay, Path path, int count)
		{
			SpawnEntry entry = Hunter(delay, path);
			entry.count = count;
			entry.interval = HUNTER_TRAIN_INTERVAL;
			return entry;
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

		SpawnEntry Swarm(float delay, Path path, int count)
		{
			const PathPoints points = GetPathPoints(path);
			return { .delay = delay, .type = EnemyType::Swarm, .position = points.start, .target = points.target, .count = count, .interval = SWARM_INTERVAL };
		}
	}

	const std::vector<Wave>& GetWaves()
	{
		static const std::vector<Wave> waves =
		{
			{
				.spawnEntries =
				{
					Hunter(3.f, Path::NorthWestToSouthEast),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					Hunter(2.f, Path::NorthEastToSouthWest),
					Hunter(2.f, Path::NorthToSouth),
					Hunter(2.f, Path::EastToWest), Hunter(0.f, Path::WestToEast),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					TurretFromRight(3.f, TURRET_HIGH_ROW),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					TurretFromRight(2.f, TURRET_HIGH_ROW),
					HunterTrain(2.f, Path::NorthWestToSouthEast, 3),
					HunterTrain(2.f, Path::NorthEastToSouthWest, 3),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					Hunter(2.f, Path::EastToWest), Hunter(0.f, Path::WestToEast),
					Hunter(1.f, Path::EastToWest), Hunter(0.f, Path::WestToEast),
					Hunter(1.f, Path::EastToWest), Hunter(0.f, Path::WestToEast),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					TurretFromLeft(2.f, TURRET_LOW_ROW), TurretFromRight(0.f, TURRET_HIGH_ROW),
					HunterTrain(1.f, Path::NorthToSouth, 3),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					Hunter(2.f, Path::NorthEastToSouthWest), Hunter(0.f, Path::NorthWestToSouthEast),
					Hunter(1.f, Path::NorthEastToSouthWest), Hunter(0.f, Path::NorthWestToSouthEast),
					TurretFromRight(2.f, TURRET_HIGH_ROW), TurretFromLeft(0.f, TURRET_LOW_ROW),
					HunterTrain(2.f, Path::NorthToSouth, 3),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					Swarm(3.f, Path::WestToEast, 5),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					TurretFromRight(2.f, TURRET_HIGH_ROW),
					TurretFromRight(3.f, TURRET_LOW_ROW), HunterTrain(0.f, Path::WestToEast, 3),
					Swarm(2.f, Path::WestToEast, 5),
					HunterTrain(2.f, Path::NorthToSouth, 3),
				},
				.pauseAfterWave = 2.f,
			},
			{
				.spawnEntries =
				{
					Charger(3.f, RIGHT_LANE_X), Charger(0.f, LEFT_LANE_X),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					HunterTrain(2.f, Path::NorthWestToSouthEast, 2),
					TurretFromRight(2.f, TURRET_HIGH_ROW),
					Charger(2.f, RIGHT_LANE_X),
					HunterTrain(3.f, Path::NorthEastToSouthWest, 2),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					TurretFromRight(2.f, TURRET_HIGH_ROW), TurretFromLeft(0.f, TURRET_LOW_ROW), HunterTrain(0.f, Path::NorthToSouth, 2),
					Charger(3.f, RIGHT_LANE_X), Charger(0.f, LEFT_LANE_X),
					Hunter(3.f, Path::NorthEastToSouthWest), Hunter(0.f, Path::NorthWestToSouthEast),
					Hunter(1.f, Path::NorthEastToSouthWest), Hunter(0.f, Path::NorthWestToSouthEast),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					Hunter(2.f, Path::EastToWest), Hunter(0.f, Path::WestToEast), Charger(0.f, RIGHT_LANE_X), Charger(0.f, LEFT_LANE_X),
					Hunter(1.f, Path::EastToWest), Hunter(0.f, Path::WestToEast),
					Swarm(2.f, Path::NorthToSouth, 5),
					TurretFromRight(3.f, TURRET_HIGH_ROW), TurretFromLeft(0.f, TURRET_LOW_ROW),
					Hunter(3.f, Path::NorthEastToSouthWest), Hunter(0.f, Path::NorthWestToSouthEast),
					Hunter(1.f, Path::NorthEastToSouthWest), Hunter(0.f, Path::NorthWestToSouthEast),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					Hunter(2.f, Path::NorthEastToSouthWest), Hunter(0.f, Path::NorthWestToSouthEast), Charger(0.f, LEFT_LANE_X),
					Hunter(1.f, Path::NorthEastToSouthWest), Hunter(0.f, Path::NorthWestToSouthEast),
					Swarm(2.f, Path::WestToEast, 5),
					TurretFromLeft(2.f, TURRET_LOW_ROW), HunterTrain(0.f, Path::WestToEast, 2),
					Charger(3.f, RIGHT_LANE_X), HunterTrain(0.f, Path::NorthToSouth, 3),
				},
				.pauseAfterWave = 2.f,
			},
			{
				.spawnEntries =
				{
					HunterTrain(3.f, Path::NorthToSouth, 3),
					TurretFromLeft(2.f, TURRET_LOW_ROW), Swarm(0.f, Path::WestToEast, 5),
					TurretFromRight(3.f, TURRET_HIGH_ROW), HunterTrain(0.f, Path::WestToEast, 2),
					Swarm(3.f, Path::EastToWest, 5), HunterTrain(0.f, Path::EastToWest, 2),
					Charger(4.f, RIGHT_LANE_X),
					HunterTrain(1.f, Path::NorthWestToSouthEast, 2),
					Charger(3.f, CENTER_X), HunterTrain(0.f, Path::EastToWest, 2),
					Charger(3.f, LEFT_LANE_X),
					HunterTrain(1.f, Path::NorthToSouth, 2),
					Hunter(3.f, Path::NorthWestToSouthEast), Hunter(0.f, Path::NorthEastToSouthWest),
					Hunter(1.f, Path::NorthWestToSouthEast), Hunter(0.f, Path::NorthEastToSouthWest),
				},
				.pauseAfterWave = 1.f,
			},
		};

		return waves;
	}
}
