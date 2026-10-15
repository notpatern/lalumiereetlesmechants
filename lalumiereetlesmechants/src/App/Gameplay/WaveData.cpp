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
					{.delay = 0.f, .position = {30.f, PlayArea::SPAWN_Y}, .target = {30.f, PlayArea::HEIGHT}},
					{.delay = 1.f, .position = {40.f, PlayArea::SPAWN_Y}, .target = {40.f, PlayArea::HEIGHT}},
					{.delay = 1.f, .position = {50.f, PlayArea::SPAWN_Y}, .target = {50.f, PlayArea::HEIGHT}},
				},
				.pauseAfterWave = 2.f,
			},
			{
				.spawnEntries =
				{
					{.delay = 0.f, .position = {20.f, PlayArea::SPAWN_Y}, .target = {20.f, PlayArea::HEIGHT}},
					{.delay = 0.f, .position = {30.f, PlayArea::SPAWN_Y}, .target = {30.f, PlayArea::HEIGHT}},
					{.delay = 0.f, .position = {40.f, PlayArea::SPAWN_Y}, .target = {40.f, PlayArea::HEIGHT}},
					{.delay = 0.f, .position = {50.f, PlayArea::SPAWN_Y}, .target = {50.f, PlayArea::HEIGHT}},
					{.delay = 0.f, .position = {60.f, PlayArea::SPAWN_Y}, .target = {60.f, PlayArea::HEIGHT}},
				},
				.pauseAfterWave = 3.f,
			},
		};

		return waves;
	}
}
