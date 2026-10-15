#include "Enemy.h"

Enemy::Enemy() : m_health(MAX_HEALTH), m_hitbox{ .size = { HITBOX_WIDTH, HITBOX_HEIGHT } }, m_observer(nullptr), m_isActive(false)
{
	this->SetSpritePosition(&m_position);
}

void Enemy::Spawn(const SpawnEntry& entry, IEnemyObserver* observer)
{
	m_position = entry.position;
	m_velocity = entry.velocity;
	m_weapon.Equip(entry.weapon);
	m_health.Reset();
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
	return { .amount = CONTACT_DAMAGE, .type = DamageType::Collision, .source = Team::Enemy, .position = m_position };
}

void Enemy::Die()
{
	RemoveFromPlay(EnemyRemovalReason::Killed);
}

void Enemy::RefreshSpriteType()
{
	m_spriteType = IsFlashing() ? SpriteUtils::SpriteType::HunterHit : SpriteUtils::SpriteType::Hunter;
}

void Enemy::Shoot(Utility::Pool<Missile>& missiles)
{
	if (Missile* missile = missiles.Acquire())
	{
		const WeaponSettings& settings = m_weapon.getSettings();

		const Utility::Vector2<float> muzzle{ m_position.x + HITBOX_WIDTH/2, m_position.y + HITBOX_HEIGHT};

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
