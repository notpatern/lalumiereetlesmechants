#include "WaveData.h"

#include "PlayArea.h"

namespace WaveData
{
	const std::vector<Wave>& GetWaves()
	{
		static const std::vector<Wave> waves =
		{
			{
				.spawnEntries =
				{
					{.delay = 0.f, .position = {30.f, PlayArea::SPAWN_Y}, .velocity = {0.f, 4.f}},
					{.delay = 1.f, .position = {40.f, PlayArea::SPAWN_Y}, .velocity = {0.f, 4.f}},
					{.delay = 1.f, .position = {50.f, PlayArea::SPAWN_Y}, .velocity = {0.f, 4.f}},
				},
				.pauseAfterWave = 2.f,
			},
			{
				.spawnEntries =
				{
					{.delay = 0.f, .position = {20.f, PlayArea::SPAWN_Y}, .velocity = {0.f, 3.f}},
					{.delay = 0.f, .position = {30.f, PlayArea::SPAWN_Y}, .velocity = {0.f, 3.f}},
					{.delay = 0.f, .position = {40.f, PlayArea::SPAWN_Y}, .velocity = {0.f, 3.f}},
					{.delay = 0.f, .position = {50.f, PlayArea::SPAWN_Y}, .velocity = {0.f, 3.f}},
					{.delay = 0.f, .position = {60.f, PlayArea::SPAWN_Y}, .velocity = {0.f, 3.f}},
				},
				.pauseAfterWave = 3.f,
			},
		};

		return waves;
	}
}
