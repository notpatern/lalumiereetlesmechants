#pragma once
#include "MissileArchetype.h"
#include "../../Utilities/Countdown.h"

enum class AimMode
{
	Down,
	AtPlayer
};

struct WeaponSettings
{
	float fireInterval = 0.f;
	AimMode aim = AimMode::Down;
	MissileType missile = MissileType::EnemyShot;
};

class Weapon
{
public:
	void Equip(const WeaponSettings& inSettings);
	[[nodiscard]] bool Update(float inDeltaTime);

	const WeaponSettings& getSettings() const { return m_settings; }

private:
	WeaponSettings m_settings;
	Utility::Countdown m_cooldown;
};
