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

		PathPoints GetPathPoints(Path inPath)
		{
			switch (inPath)
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

		SpawnEntry MakeHunter(float inDelay, Path inPath)
		{
			const PathPoints points = GetPathPoints(inPath);
			return { .delay = inDelay, .type = EnemyType::Hunter, .position = points.start, .target = points.target };
		}

		SpawnEntry MakeHunterTrain(float inDelay, Path inPath, int inCount)
		{
			SpawnEntry entry = MakeHunter(inDelay, inPath);
			entry.count = inCount;
			entry.interval = HUNTER_TRAIN_INTERVAL;
			return entry;
		}

		SpawnEntry MakeTurretFromLeft(float inDelay, float inRow)
		{
			return { .delay = inDelay, .type = EnemyType::Turret, .position = { TURRET_LEFT_X, inRow }, .velocity = { TURRET_SPEED, 0.f } };
		}

		SpawnEntry MakeTurretFromRight(float inDelay, float inRow)
		{
			return { .delay = inDelay, .type = EnemyType::Turret, .position = { TURRET_RIGHT_X, inRow }, .velocity = { -TURRET_SPEED, 0.f } };
		}

		SpawnEntry MakeCharger(float inDelay, float inX)
		{
			return { .delay = inDelay, .type = EnemyType::Charger, .position = { inX, PlayArea::SPAWN_Y }, .target = { inX, CHARGER_STOP_ROW } };
		}

		SpawnEntry MakeSwarm(float inDelay, Path inPath, int inCount)
		{
			const PathPoints points = GetPathPoints(inPath);
			return { .delay = inDelay, .type = EnemyType::Swarm, .position = points.start, .target = points.target, .count = inCount, .interval = SWARM_INTERVAL };
		}
	}

	const std::vector<Wave>& GetWaves()
	{
		static const std::vector<Wave> WAVES =
		{
			{
				.spawnEntries =
				{
					MakeHunter(3.f, Path::NorthWestToSouthEast),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					MakeHunter(2.f, Path::NorthEastToSouthWest),
					MakeHunter(2.f, Path::NorthToSouth),
					MakeHunter(2.f, Path::EastToWest), MakeHunter(0.f, Path::WestToEast),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					MakeTurretFromRight(3.f, TURRET_HIGH_ROW),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					MakeTurretFromRight(2.f, TURRET_HIGH_ROW),
					MakeHunterTrain(2.f, Path::NorthWestToSouthEast, 3),
					MakeHunterTrain(2.f, Path::NorthEastToSouthWest, 3),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					MakeHunter(2.f, Path::EastToWest), MakeHunter(0.f, Path::WestToEast),
					MakeHunter(1.f, Path::EastToWest), MakeHunter(0.f, Path::WestToEast),
					MakeHunter(1.f, Path::EastToWest), MakeHunter(0.f, Path::WestToEast),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					MakeTurretFromLeft(2.f, TURRET_LOW_ROW), MakeTurretFromRight(0.f, TURRET_HIGH_ROW),
					MakeHunterTrain(1.f, Path::NorthToSouth, 3),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					MakeHunter(2.f, Path::NorthEastToSouthWest), MakeHunter(0.f, Path::NorthWestToSouthEast),
					MakeHunter(1.f, Path::NorthEastToSouthWest), MakeHunter(0.f, Path::NorthWestToSouthEast),
					MakeTurretFromRight(2.f, TURRET_HIGH_ROW), MakeTurretFromLeft(0.f, TURRET_LOW_ROW),
					MakeHunterTrain(2.f, Path::NorthToSouth, 3),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					MakeSwarm(3.f, Path::WestToEast, 5),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					MakeTurretFromRight(2.f, TURRET_HIGH_ROW),
					MakeTurretFromRight(3.f, TURRET_LOW_ROW), MakeHunterTrain(0.f, Path::WestToEast, 3),
					MakeSwarm(2.f, Path::WestToEast, 5),
					MakeHunterTrain(2.f, Path::NorthToSouth, 3),
				},
				.pauseAfterWave = 2.f,
			},
			{
				.spawnEntries =
				{
					MakeCharger(3.f, RIGHT_LANE_X), MakeCharger(0.f, LEFT_LANE_X),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					MakeHunterTrain(2.f, Path::NorthWestToSouthEast, 2),
					MakeTurretFromRight(2.f, TURRET_HIGH_ROW),
					MakeCharger(2.f, RIGHT_LANE_X),
					MakeHunterTrain(3.f, Path::NorthEastToSouthWest, 2),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					MakeTurretFromRight(2.f, TURRET_HIGH_ROW), MakeTurretFromLeft(0.f, TURRET_LOW_ROW), MakeHunterTrain(0.f, Path::NorthToSouth, 2),
					MakeCharger(3.f, RIGHT_LANE_X), MakeCharger(0.f, LEFT_LANE_X),
					MakeHunter(3.f, Path::NorthEastToSouthWest), MakeHunter(0.f, Path::NorthWestToSouthEast),
					MakeHunter(1.f, Path::NorthEastToSouthWest), MakeHunter(0.f, Path::NorthWestToSouthEast),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					MakeHunter(2.f, Path::EastToWest), MakeHunter(0.f, Path::WestToEast), MakeCharger(0.f, RIGHT_LANE_X), MakeCharger(0.f, LEFT_LANE_X),
					MakeHunter(1.f, Path::EastToWest), MakeHunter(0.f, Path::WestToEast),
					MakeSwarm(2.f, Path::NorthToSouth, 5),
					MakeTurretFromRight(3.f, TURRET_HIGH_ROW), MakeTurretFromLeft(0.f, TURRET_LOW_ROW),
					MakeHunter(3.f, Path::NorthEastToSouthWest), MakeHunter(0.f, Path::NorthWestToSouthEast),
					MakeHunter(1.f, Path::NorthEastToSouthWest), MakeHunter(0.f, Path::NorthWestToSouthEast),
				},
				.pauseAfterWave = 1.f,
			},
			{
				.spawnEntries =
				{
					MakeHunter(2.f, Path::NorthEastToSouthWest), MakeHunter(0.f, Path::NorthWestToSouthEast), MakeCharger(0.f, LEFT_LANE_X),
					MakeHunter(1.f, Path::NorthEastToSouthWest), MakeHunter(0.f, Path::NorthWestToSouthEast),
					MakeSwarm(2.f, Path::WestToEast, 5),
					MakeTurretFromLeft(2.f, TURRET_LOW_ROW), MakeHunterTrain(0.f, Path::WestToEast, 2),
					MakeCharger(3.f, RIGHT_LANE_X), MakeHunterTrain(0.f, Path::NorthToSouth, 3),
				},
				.pauseAfterWave = 2.f,
			},
			{
				.spawnEntries =
				{
					MakeHunterTrain(3.f, Path::NorthToSouth, 4),
					MakeSwarm(0.f, Path::WestToEast, 5), MakeSwarm(0.f, Path::EastToWest, 5),
					MakeTurretFromLeft(2.f, TURRET_LOW_ROW), MakeTurretFromRight(0.f, TURRET_HIGH_ROW),
					MakeHunterTrain(0.f, Path::NorthWestToSouthEast, 3), MakeHunterTrain(0.f, Path::NorthEastToSouthWest, 3),
					MakeCharger(3.f, LEFT_LANE_X), MakeCharger(0.f, CENTER_X), MakeCharger(0.f, RIGHT_LANE_X),
					MakeHunterTrain(1.f, Path::WestToEast, 3), MakeHunterTrain(0.f, Path::EastToWest, 3),
					MakeSwarm(3.f, Path::NorthToSouth, 8),
					MakeTurretFromRight(2.f, TURRET_LOW_ROW), MakeTurretFromLeft(0.f, TURRET_HIGH_ROW),
					MakeHunterTrain(0.f, Path::NorthToSouth, 4),
					MakeCharger(3.f, LEFT_LANE_X), MakeCharger(0.f, RIGHT_LANE_X),
					MakeSwarm(1.f, Path::WestToEast, 6), MakeSwarm(0.f, Path::EastToWest, 6),
					MakeHunterTrain(1.f, Path::NorthWestToSouthEast, 4), MakeHunterTrain(0.f, Path::NorthEastToSouthWest, 4),
				},
				.pauseAfterWave = 1.f,
			},
		};

		return WAVES;
	}
}
