#pragma once
#include <vector>

#include "Wave.h"
#include "../../Utilities/Pool.h"
#include "../../Utilities/Countdown.h"
#include "../Entites/Enemy.h"
#include <cstddef>
#include "../Gameplay/IEnemyObserver.h"

class WaveSpawner : public IEnemyObserver
{
public:
	WaveSpawner(const std::vector<Wave>& waves, Utility::Pool<Enemy>& enemies);
	WaveSpawner(std::vector<Wave>&&, Utility::Pool<Enemy>&) = delete;

	WaveSpawner(const WaveSpawner&) = delete;
	WaveSpawner& operator=(const WaveSpawner&) = delete;

	void Update(float deltaTime);

	void OnEnemyRemoved(const EnemyRemovedEvent& event) override;

	bool IsFinished() const {return m_state == State::Finished;}
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

	void StartWave(std::size_t waveIndex);
	void UpdateSpawning(float deltaTime);
	void SpawnEnemy(const SpawnEntry& entry);


};
