#pragma once
#include "../../Utilities/Countdown.h"

struct WeaponSettings
{
	float fireInterval = 0.f;
	float missileSpeed = 20.f;
	int missileDamage = 1;
};

class Weapon
{
public:
	void Equip(const WeaponSettings& settings);
	bool Update(float deltaTime);

	const WeaponSettings& getSettings() const { return m_settings; }

private:
	WeaponSettings m_settings;
	Utility::Countdown m_cooldown;
};
