#include "Enemy.h"

#include "../../Utilities/ScreenDirection.h"
#include "../Gameplay/PlayArea.h"

Enemy::Enemy() : m_type(EnemyType::Hunter), m_health(0), m_observer(nullptr)
{
}

void Enemy::Spawn(const SpawnEntry& entry, IEnemyObserver* observer)
{
	const EnemyArchetype& archetype = EnemyArchetypes::Get(entry.type);

	m_type = entry.type;
	m_position = entry.position;
	m_movement.Start(archetype.movement, entry.position, entry.velocity, entry.target, archetype.hitboxSize);
	m_health = Health(archetype.maxHealth);
	m_hitbox.size = archetype.hitboxSize;
	m_weapon.Equip(archetype.weapon);
	m_hitFlash.Start(0.f);
	m_observer = observer;
	RefreshSpriteType();
	Activate();
}

void Enemy::Update(float dt, const EnemyUpdateContext& context)
{
	if (!getIsActive())
	{
		return;
	}

	m_position = m_movement.Update(dt, context.playerCenter);

	m_hitFlash.Update(dt);

	RefreshSpriteType();

	if (m_weapon.Update(dt) && PlayArea::Bounds().Contains(getHitboxRect()))
	{
		Shoot(context);
	}
}

void Enemy::OnLeftPlayArea()
{
	RemoveFromPlay(EnemyRemovalReason::Escaped);
}

DamageResult Enemy::TakeDamage(const DamageInfos& damage)
{
	if (!getIsActive())
	{
		return DamageResult::Ignored;
	}

	if (m_health.TakeDamage(damage.amount))
	{
		Die();
		return DamageResult::Killed;
	}

	m_hitFlash.Start(HIT_FLASH_DURATION);
	RefreshSpriteType();
	return DamageResult::Hit;
}

DamageInfos Enemy::getContactDamage() const
{
	return { .amount = EnemyArchetypes::Get(m_type).contactDamage, .type = DamageType::Collision, .source = Team::Enemy, .position = m_position };
}

void Enemy::Die()
{
	RemoveFromPlay(EnemyRemovalReason::Killed);
}

void Enemy::RefreshSpriteType()
{
	const EnemyArchetype& archetype = EnemyArchetypes::Get(m_type);

	if (IsFlashing())
	{
		m_spriteType = archetype.hitSprite;
	}
	else if (IsTelegraphing())
	{
		m_spriteType = archetype.telegraphSprite;
	}
	else
	{
		m_spriteType = archetype.sprite;
	}
}

void Enemy::Shoot(const EnemyUpdateContext& context)
{
	if (Missile* missile = context.missiles.Acquire())
	{
		const WeaponSettings& settings = m_weapon.getSettings();

		const Utility::Vector2<float> muzzle{ m_position.x + m_hitbox.size.x / 2 - settings.missileSize.x / 2, m_position.y + m_hitbox.size.y };

		Utility::Vector2<float> direction{ 0.f, 1.f };
		if (settings.aim == AimMode::AtPlayer)
		{
			direction = Utility::ScreenDirection(muzzle, context.playerCenter);
		}

		missile->Launch(muzzle, { direction.x * settings.missileSpeed, direction.y * settings.missileSpeed }, Team::Enemy, settings.missileDamage, settings.missileSize);
	}
}

void Enemy::RemoveFromPlay(EnemyRemovalReason reason)
{
	if (!getIsActive())
	{
		return;
	}

	Deactivate();

	if (m_observer)
	{
		m_observer->OnEnemyRemoved({.reason = reason, .position = m_position});
		m_observer = nullptr;
	}

}
