#pragma once
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Vector2.h"

enum class AimMode
{
	Down,
	AtPlayer
};

struct WeaponSettings
{
	float fireInterval = 0.f;
	float missileSpeed = 20.f;
	int missileDamage = 1;
	AimMode aim = AimMode::Down;
	Utility::Vector2<int> missileSize{ 1, 1 };
};

class Weapon
{
public:
	void Equip(const WeaponSettings& settings);
	[[nodiscard]] bool Update(float deltaTime);

	const WeaponSettings& getSettings() const { return m_settings; }

private:
	WeaponSettings m_settings;
	Utility::Countdown m_cooldown;
};
