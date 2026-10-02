#include "Health.h"

Health::Health(int maxHealth) : m_max(maxHealth), m_current(maxHealth)
{

}

bool Health::TakeDamage(int amount)
{
	if (IsDead() || amount <= 0)
	{
		return false;
	}

	m_current -= amount;
	if (m_current < 0)
	{
		m_current = 0;
	}

	return IsDead();
}

void Health::Reset()
{
	m_current = m_max;
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
