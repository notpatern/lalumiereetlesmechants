#pragma once
#include <vector>

#include "../../Utilities/Vector2.h"

struct SpawnEntry
{
	float delay = 0.0f;
	Utility::Vector2<float> position;
	Utility::Vector2<float> velocity;
};

struct Wave
{
	std::vector<SpawnEntry> spawnEntries;
	float pauseAfterWave = 2.0f;
};
