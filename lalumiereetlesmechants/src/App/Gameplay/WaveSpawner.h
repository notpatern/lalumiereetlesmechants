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

	void Update(float deltaTime);

	void OnEnemyRemoved(const EnemyRemovedEvent& event) override;

private:
	enum class State
	{
		Spawning,
		WaitingForClear,
		Pause
	};

	const std::vector<Wave>* m_waves;
	Utility::Pool<Enemy>* m_enemies;
	State m_state;
	std::size_t m_waveIndex;
	std::size_t m_entryIndex;
	std::size_t m_aliveCount;
	Utility::Countdown m_timer;

	void StartWave(std::size_t waveIndex);
	void UpdateSpawning(float deltaTime);
	void SpawnEnemy(const SpawnEntry& entry);


};
