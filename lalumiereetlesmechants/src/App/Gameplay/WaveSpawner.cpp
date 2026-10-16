#include "WaveSpawner.h"

WaveSpawner::WaveSpawner(const std::vector<Wave>& waves, Utility::Pool<Enemy>& enemies)
	: m_waves(&waves), m_enemies(&enemies), m_state(State::Spawning), m_waveIndex(0), m_entryIndex(0), m_spawnedFromEntry(0), m_aliveCount(0)
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
			if (m_waveIndex + 1 < m_waves->size())
			{
				StartWave(m_waveIndex + 1);
			}
			else
			{
				m_state = State::Finished;
			}
		}
		break;

	case State::Finished:
		break;
	}
}

void WaveSpawner::OnEnemyRemoved(const EnemyRemovedEvent&)
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
		const SpawnEntry& entry = entries[m_entryIndex];
		SpawnEnemy(entry);
		++m_spawnedFromEntry;

		if (m_spawnedFromEntry < entry.count)
		{
			m_timer.Chain(entry.interval);
			continue;
		}

		m_spawnedFromEntry = 0;
		++m_entryIndex;

		if (m_entryIndex < entries.size())
		{
			m_timer.Chain(entries[m_entryIndex].delay);
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
		enemy->Spawn(entry, this);
		++m_aliveCount;
	}
}
