#pragma once
#include "Missile.h"
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Vector2.h"
#include "../Gameplay/Collision.h"
#include "../../Utilities/Pool.h"
#include "../Gameplay/Health.h"
#include "../Gameplay/IDamageable.h"
#include "../Gameplay/IEnemyObserver.h"
#include "../Gameplay/Movement.h"
#include "../Gameplay/Wave.h"
#include "Entities.h"
#include "../Gameplay/EnemyArchetype.h"
#include "../Gameplay/Weapon.h"

struct EnemyUpdateContext
{
	Utility::Pool<Missile>& missiles;
	Utility::Vector2<float> playerCenter;
};


class Enemy : public IDamageable, public Entity
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

	int getPoints() const { return EnemyArchetypes::Get(m_type).points; }

	bool getIsActive() const { return m_isActive; }
	EnemyType getType() const { return m_type; }
	Utility::Vector2<float> getCurrentPosition() const { return m_position; }
	Collision::CellRect getHitboxRect() const { return m_hitbox.At(m_position); }
	bool IsFlashing() const { return !m_hitFlash.IsFinished(); }

	Team getTeam() const { return Team::Enemy; }

private:
	static constexpr float HIT_FLASH_DURATION = 0.1f;

	EnemyType m_type;

	Movement m_movement;

	Health m_health;
	Collision::Hitbox m_hitbox;
	Utility::Countdown m_hitFlash;
	Weapon m_weapon;

	IEnemyObserver* m_observer;

	bool m_isActive;

	void Die();

	void RefreshSpriteType();

	void Shoot(const EnemyUpdateContext& context);

	void RemoveFromPlay(EnemyRemovalReason reason);
};
