#pragma once
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Vector2.h"
#include "../Gameplay/DamageInfos.h"
#include "../Gameplay/IDamageable.h"

class Missile
{
public:
	Missile();

	Utility::Vector2<float> getCurrentPosition() const { return m_currentPosition; }
	Utility::Vector2<float> getLastPosition() const { return m_lastPosition; }

	void Update(float dt);

	bool getIsActive() const { return m_isActive; }

	void Launch(const Utility::Vector2<float>& position, const Utility::Vector2<float>& velocity, Team team, int damage);
	void Explode();

	DamageResult OnHit(IDamageable& damageable);

	Team getTeam() const { return m_team; }
	DamageInfos getDamageInfos() const;
	void OnLeftPlayArea();

private:
	Utility::Vector2<float> m_currentPosition;
	Utility::Vector2<float> m_lastPosition;
	Utility::Vector2<float> m_velocity;

	static constexpr float LIFETIME = 10.f;
	Utility::Countdown m_lifetime;

	Team m_team;
	int m_damage;

	bool m_isActive;

	void Deactivate();

};

