#include "Weapon.h"

void Weapon::Equip(const WeaponSettings& settings)
{
	m_settings = settings;
	m_cooldown.Start(settings.fireInterval);
}

bool Weapon::Update(float deltaTime)
{
	if (m_settings.fireInterval <= 0.0f)
	{
		return false;
	}

	m_cooldown.Update(deltaTime);
	if (!m_cooldown.IsFinished())
	{
		return false;
	}

	m_cooldown.Chain(m_settings.fireInterval);
	return true;
}
