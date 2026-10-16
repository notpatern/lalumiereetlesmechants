#include "Weapon.h"

void Weapon::Equip(const WeaponSettings& inSettings)
{
	m_settings = inSettings;
	m_cooldown.Start(inSettings.fireInterval);
}

bool Weapon::Update(float inDeltaTime)
{
	if (m_settings.fireInterval <= 0.f)
	{
		return false;
	}

	m_cooldown.Update(inDeltaTime);
	if (!m_cooldown.IsFinished())
	{
		return false;
	}

	m_cooldown.Chain(m_settings.fireInterval);
	return true;
}
