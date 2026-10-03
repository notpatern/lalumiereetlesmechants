#include "WaveSpawner.h"

WaveSpawner::WaveSpawner(const std::vector<Wave>& waves, Utility::Pool<Enemy>& enemies)
	: m_waves(&waves), m_enemies(&enemies), m_state(State::Spawning), m_waveIndex(0), m_entryIndex(0),  m_aliveCount(0)
{
	if (!m_waves->empty())
	{
		StartWave(0);
	}
}

void WaveSpawner::Update(float deltaTime)
{
	if (m_waves->empty())
	{
		return;
	}

	switch (m_state)
	{
	case State::Spawning:
		UpdateSpawning(deltaTime);
		break;

	case State::WaitingForClear:
		if (m_aliveCount == 0)
		{
			m_state = State::Pause;
			m_timer.Start((*m_waves)[m_waveIndex].pauseAfterWave);
		}
		break;

	case State::Pause:
		m_timer.Update(deltaTime);
		if (m_timer.IsFinished())
		{
			StartWave((m_waveIndex + 1) % m_waves->size()); //TODO : Voir si on veut vraiment boucler ou win screen / boss
		}
		break;
	}
}

void WaveSpawner::OnEnemyRemoved(const EnemyRemovedEvent& event)
{
	if (m_aliveCount > 0)
	{
		--m_aliveCount;
	}
}

void WaveSpawner::StartWave(std::size_t waveIndex)
{
	m_waveIndex = waveIndex;
	m_entryIndex = 0;
	m_state = State::Spawning;

	const std::vector<SpawnEntry>& entries = (*m_waves)[m_waveIndex].spawnEntries;

	if (!entries.empty())
	{
		m_timer.Start(entries[0].delay);
	}
}

void WaveSpawner::UpdateSpawning(float deltaTime)
{
	const std::vector<SpawnEntry>& entries = (*m_waves)[m_waveIndex].spawnEntries;

	m_timer.Update(deltaTime);

	while (m_entryIndex < entries.size() && m_timer.IsFinished())
	{
		SpawnEnemy(entries[m_entryIndex]);
		++m_entryIndex;

		if (m_entryIndex < entries.size())
		{
			m_timer.Start(entries[m_entryIndex].delay);
		}
	}

	if (m_entryIndex >= entries.size())
	{
		m_state = State::WaitingForClear;
	}
}

void WaveSpawner::SpawnEnemy(const SpawnEntry& entry)
{
	if (Enemy* enemy = m_enemies->Acquire())
	{
		enemy->Spawn(entry.position, entry.velocity, this);
		++m_aliveCount;
	}
}
