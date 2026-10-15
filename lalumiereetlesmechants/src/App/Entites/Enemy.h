#pragma once
#include "Missile.h"
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Vector2.h"
#include "../Gameplay/Collision.h"
#include "../../Utilities/Pool.h"
#include "../Gameplay/Health.h"
#include "../Gameplay/IDamageable.h"
#include "../Gameplay/IEnemyObserver.h"
#include "../Gameplay/Wave.h"

struct EnemyUpdateContext
{
	Utility::Pool<Missile>& missiles;
};


class Enemy : public IDamageable, Sprite
{
public:
	Enemy();

	void Spawn(const SpawnEntry& entry, IEnemyObserver* observer);
	void Update(float dt, const EnemyUpdateContext& context);
	void OnLeftPlayArea();

	// IDamageable
	DamageResult TakeDamage(const DamageInfos& damage) override;
	bool IsDead() const override { return m_health.IsDead(); }

	DamageInfos getContactDamage() const;

	bool getIsActive() const { return m_isActive; }
	Utility::Vector2<float> getCurrentPosition() const { return m_currentPosition; }
	Collision::CellRect getHitboxRect() const { return m_hitbox.At(m_currentPosition); }
	bool IsFlashing() const { return !m_hitFlash.IsFinished(); }

	Team getTeam() const { return Team::Enemy; }

private:
	static constexpr int MAX_HEALTH = 3;
	static constexpr float HIT_FLASH_DURATION = 0.1f;

	static constexpr int HITBOX_WIDTH = 3;
	static constexpr int HITBOX_HEIGHT = 2;

	static constexpr int CONTACT_DAMAGE = 1;

	Utility::Vector2<float> m_currentPosition;
	Utility::Vector2<float> m_velocity;

	Health m_health;
	Collision::Hitbox m_hitbox;
	Utility::Countdown m_hitFlash;
	Weapon m_weapon;

	IEnemyObserver* m_observer;

	bool m_isActive;

	void Die();

	void Shoot(Utility::Pool<Missile>& missiles);

	void RemoveFromPlay(EnemyRemovalReason reason);
};
