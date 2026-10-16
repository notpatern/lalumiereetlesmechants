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
#include "PooledEntity.h"
#include "../Gameplay/EnemyArchetype.h"
#include "../Gameplay/Weapon.h"

struct EnemyUpdateContext
{
	Utility::Pool<Missile>& missiles;
	Utility::Vector2<float> playerCenter;
};


class Enemy : public IDamageable, public PooledEntity
{
public:
	Enemy();

	void Spawn(const SpawnEntry& entry, IEnemyObserver* observer);
	void Update(float dt, const EnemyUpdateContext& context);
	void OnLeftPlayArea();
	void Nudge(const Utility::Vector2<float>& offset) { m_movement.Nudge(offset); }

	// IDamageable
	DamageResult TakeDamage(const DamageInfos& damage) override;
	bool IsDead() const override { return m_health.IsDead(); }

	DamageInfos getContactDamage() const;

	int getPoints() const { return EnemyArchetypes::Get(m_type).points; }
	int getXp() const { return EnemyArchetypes::Get(m_type).xp; }
	const MovementSettings& getMovementSettings() const { return EnemyArchetypes::Get(m_type).movement; }

	EnemyType getType() const { return m_type; }
	Collision::CellRect getHitboxRect() const { return m_hitbox.At(m_position); }
	bool IsFlashing() const { return !m_hitFlash.IsFinished(); }
	bool IsTelegraphing() const { return m_movement.IsTelegraphing(); }

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

	void Die();

	void RefreshSpriteType();

	void Shoot(const EnemyUpdateContext& context);

	void RemoveFromPlay(EnemyRemovalReason reason);
};
