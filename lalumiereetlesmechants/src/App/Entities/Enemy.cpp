#include "Enemy.h"
#include "../../Utilities/ScreenDirection.h"
#include "../Gameplay/PlayArea.h"

Enemy::Enemy() : m_type(EnemyType::Hunter), m_health(0), m_observer(nullptr)
{
}

void Enemy::Spawn(const SpawnEntry& inEntry, IEnemyObserver* inObserver)
{
	const EnemyArchetype& archetype = EnemyArchetypes::Get(inEntry.type);

	m_type = inEntry.type;
	m_position = inEntry.position;
	m_movement.Start(archetype.movement, inEntry.position, inEntry.velocity, inEntry.target, archetype.hitboxSize);
	m_health = Health(archetype.maxHealth);
	m_hitbox.size = archetype.hitboxSize;
	m_weapon.Equip(archetype.weapon);
	m_hitFlash.Start(0.f);
	m_observer = inObserver;
	RefreshSpriteType();
	Activate();
}

void Enemy::Update(float inDeltaTime, const EnemyUpdateContext& inContext)
{
	m_position = m_movement.Update(inDeltaTime, inContext.playerCenter);

	m_hitFlash.Update(inDeltaTime);

	RefreshSpriteType();

	if (m_weapon.Update(inDeltaTime) && PlayArea::Bounds().ContainsRect(getHitboxRect()))
	{
		Shoot(inContext);
	}
}

void Enemy::OnLeftPlayArea()
{
	RemoveFromPlay(EnemyRemovalReason::Escaped);
}

DamageResult Enemy::TakeDamage(const DamageInfos& inDamage)
{
	if (!IsActive())
	{
		return DamageResult::Ignored;
	}

	if (m_health.TakeDamage(inDamage.amount))
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

void Enemy::Shoot(const EnemyUpdateContext& inContext) const
{
	if (Missile* missile = inContext.missiles.Acquire())
	{
		const WeaponSettings& settings = m_weapon.getSettings();
		const MissileArchetype& missileArchetype = MissileArchetypes::Get(settings.missile);

		const Utility::Vector2<float> muzzle{ m_position.x + m_hitbox.size.x / 2 - missileArchetype.hitboxSize.x / 2, m_position.y + m_hitbox.size.y };

		Utility::Vector2<float> direction{ 0.f, 1.f };
		if (settings.aim == AimMode::AtPlayer)
		{
			direction = Utility::ScreenDirection(muzzle, inContext.playerCenter);
		}

		missile->Launch(settings.missile, muzzle, { direction.x * missileArchetype.speed, direction.y * missileArchetype.speed });
	}
}

void Enemy::Die()
{
	RemoveFromPlay(EnemyRemovalReason::Killed);
}

void Enemy::RemoveFromPlay(EnemyRemovalReason inReason)
{
	if (!IsActive())
	{
		return;
	}

	Deactivate();

	if (m_observer)
	{
		m_observer->OnEnemyRemoved({ .reason = inReason, .position = m_position });
		m_observer = nullptr;
	}
}
