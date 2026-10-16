#include "Experience.h"

void Experience::AddXp(int amount)
{
	m_xpInLevel += amount;

	while (!IsMaxLevel() && m_xpInLevel >= getXpForNextLevel())
	{
		m_xpInLevel -= getXpForNextLevel();
		++m_level;
	}
}

int Experience::getXpForNextLevel() const
{
	if (IsMaxLevel())
	{
		return 0;
	}

	return XP_FOR_NEXT_LEVEL[m_level - 1];
}
