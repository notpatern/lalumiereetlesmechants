#include "Health.h"
#include <algorithm>

Health::Health(int inMaxHealth) : m_max(inMaxHealth), m_current(inMaxHealth)
{
}

bool Health::TakeDamage(int inAmount)
{
	if (IsDead() || inAmount <= 0)
	{
		return false;
	}

	m_current -= inAmount;
	if (m_current < 0)
	{
		m_current = 0;
	}

	return IsDead();
}

void Health::Heal(int inAmount)
{
	if (IsDead() || inAmount <= 0)
	{
		return;
	}

	m_current = std::min(m_current + inAmount, m_max);
}

bool Health::IsDead() const
{
	return m_current <= 0;
}

int Health::getCurrent() const
{
	return m_current;
}

int Health::getMax() const
{
	return m_max;
}
