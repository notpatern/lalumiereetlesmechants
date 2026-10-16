#include "Experience.h"

void Experience::AddXp(int inAmount)
{
	m_xpInLevel += inAmount;

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
