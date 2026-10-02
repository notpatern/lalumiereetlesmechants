#pragma once
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Vector2.h"
#include "../Gameplay/Health.h"
#include "../Gameplay/IDamageable.h"

class Enemy : public IDamageable
{
public:
	Enemy();

	void Spawn(const Utility::Vector2<float>& position, const Utility::Vector2<float>& velocity);
	void Update(float dt);
	void Deactivate();

	// IDamageable
	void TakeDamage(const DamageInfos& damage) override;
	bool IsDead() const override { return m_health.IsDead(); }

	bool getIsActive() const { return m_isActive; }
	Utility::Vector2<float> getPosition() const { return m_position; }
	bool IsFlashing() const { return !m_hitFlash.IsFinished(); }

private:
	static constexpr int MAX_HEALTH = 3;
	static constexpr float HIT_FLASH_DURATION = 0.1f;

	Utility::Vector2<float> m_position;
	Utility::Vector2<float> m_velocity;
	Health m_health;
	Utility::Countdown m_hitFlash;
	bool m_isActive;

	void Die();
};
