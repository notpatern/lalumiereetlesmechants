#pragma once
#include <vector>
#include "EnemyArchetype.h"
#include "../../Utilities/Vector2.h"

struct SpawnEntry
{
	float delay = 0.0f;
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
	float pauseAfterWave = 2.0f;
};
