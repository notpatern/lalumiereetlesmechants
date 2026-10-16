#pragma once
#include "EnemyArchetype.h"
#include "../../Utilities/Vector2.h"
#include <vector>

struct SpawnEntry
{
	float delay = 0.f;
	EnemyType type = EnemyType::Hunter;
	Utility::Vector2<float> position;
	Utility::Vector2<float> velocity;
	Utility::Vector2<float> target;
	int count = 1;
	float interval = 0.f;
};

struct Wave
{
	std::vector<SpawnEntry> spawnEntries;
	float pauseAfterWave = 2.f;
};
