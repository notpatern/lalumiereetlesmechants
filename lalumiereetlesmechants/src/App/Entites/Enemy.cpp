#include "Enemy.h"

Enemy::Enemy() : m_type(EnemyType::Hunter), m_health(0), m_observer(nullptr), m_isActive(false)
{
	this->SetSpritePosition(&m_position);
}

void Enemy::Spawn(const SpawnEntry& entry, IEnemyObserver* observer)
{
	const EnemyArchetype& archetype = EnemyArchetypes::Get(entry.type);

	m_type = entry.type;
	m_position = entry.position;
	m_velocity = entry.velocity;
	m_health = Health(archetype.maxHealth);
	m_hitbox.size = archetype.hitboxSize;
	m_weapon.Equip(archetype.weapon);
	m_hitFlash.Start(0.f);
	m_isActive = true;
	m_observer = observer;
	RefreshSpriteType();
	SetInRenderQueue(true);
}

void Enemy::Update(float dt, const EnemyUpdateContext& context)
{
	if (!m_isActive)
	{
		return;
	}

	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;

	m_hitFlash.Update(dt);

	RefreshSpriteType();

	if (m_weapon.Update(dt))
	{
		Shoot(context.missiles);
	}
}

void Enemy::OnLeftPlayArea()
{
	RemoveFromPlay(EnemyRemovalReason::Escaped);
}

DamageResult Enemy::TakeDamage(const DamageInfos& damage)
{
	if (!m_isActive)
	{
		return DamageResult::Ignored;
	}

	if (m_health.TakeDamage(damage.amount))
	{
		Die();
		return DamageResult::Killed;
	}

	m_hitFlash.Start(HIT_FLASH_DURATION);
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
	m_spriteType = IsFlashing() ? archetype.hitSprite : archetype.sprite;
}

void Enemy::Shoot(Utility::Pool<Missile>& missiles)
{
	if (Missile* missile = missiles.Acquire())
	{
		const WeaponSettings& settings = m_weapon.getSettings();

		const Utility::Vector2<float> muzzle{ m_position.x + m_hitbox.size.x / 2, m_position.y + m_hitbox.size.y };

		missile->Launch(muzzle, {0.f, settings.missileSpeed}, Team::Enemy, settings.missileDamage);
	}

}

void Enemy::RemoveFromPlay(EnemyRemovalReason reason)
{
	if (!m_isActive)
	{
		return;
	}

	m_isActive = false;
	SetInRenderQueue(false);

	if (m_observer)
	{
		m_observer->OnEnemyRemoved({.reason = reason, .position = m_position});
		m_observer = nullptr;
	}

}
