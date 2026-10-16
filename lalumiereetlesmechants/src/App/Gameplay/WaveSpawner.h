#pragma once
#include "IEnemyObserver.h"
#include "Wave.h"
#include "../Entites/Enemy.h"
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Pool.h"
#include <cstddef>
#include <vector>

class WaveSpawner : public IEnemyObserver
{
public:
	WaveSpawner(const std::vector<Wave>& inWaves, Utility::Pool<Enemy>& inEnemies);
	WaveSpawner(std::vector<Wave>&&, Utility::Pool<Enemy>&) = delete;

	WaveSpawner(const WaveSpawner&) = delete;
	WaveSpawner& operator=(const WaveSpawner&) = delete;

	void Update(float inDeltaTime);
	void SkipToNextWave();

	void OnEnemyRemoved(const EnemyRemovedEvent& inEvent) override;

	bool IsFinished() const { return m_state == State::Finished; }
	int getWaveNumber() const { return static_cast<int>(m_waveIndex) + 1; }
	int getWaveCount() const { return static_cast<int>(m_waves->size()); }

private:
	enum class State
	{
		Spawning,
		WaitingForClear,
		Pause,
		Finished
	};

	const std::vector<Wave>* m_waves;
	Utility::Pool<Enemy>* m_enemies;

	State m_state;
	std::size_t m_waveIndex;
	std::size_t m_entryIndex;
	int m_spawnedFromEntry;
	std::size_t m_aliveCount;
	Utility::Countdown m_timer;

	void StartWave(std::size_t inWaveIndex);
	void StartNextWave();

	void UpdateSpawning(float inDeltaTime);
	void SpawnEnemy(const SpawnEntry& inEntry);
};
