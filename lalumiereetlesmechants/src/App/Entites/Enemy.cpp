#include "Enemy.h"

Enemy::Enemy() : m_health(MAX_HEALTH), m_hitbox{ .size = { HITBOX_WIDTH, HITBOX_HEIGHT } }, m_observer(nullptr), m_isActive(false),
//	for (int y = 0; y < m_size.y; y++)
//	{
//		for (int x = 0; x < m_size.x; x++)
//		{
//			if (CanAccessMap(world,
//				Utility::Vector2<int>(m_position.x + x, m_position.y + y))
//				)
//			{
//				SetMapValue(world, Utility::Vector2<int>(
//					m_position.x + x, m_position.y + y),
//					m_sprite[y][x],
//					world.getWorldMap());
//			}
//		}
//	}
Sprite(SpriteType::Enemy)
{
	this->SetSpritePosition(Collision::ToCell(m_currentPosition));	
}

void Enemy::Spawn(const SpawnEntry& entry, IEnemyObserver* observer)
{
	m_currentPosition = entry.position;
	m_velocity = entry.velocity;
	m_weapon.Equip(entry.weapon);
	m_health.Reset();
	m_hitFlash.Start(0.f);
	m_isActive = true;
	m_observer = observer;
}

void Enemy::Update(float dt, const EnemyUpdateContext& context)
{
	if (!m_isActive)
	{
		return;
	}

	m_currentPosition.x += m_velocity.x * dt;
	m_currentPosition.y += m_velocity.y * dt;

	m_hitFlash.Update(dt);


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
	return { .amount = CONTACT_DAMAGE, .type = DamageType::Collision, .source = Team::Enemy, .position = m_currentPosition };
}

void Enemy::Die()
{
	RemoveFromPlay(EnemyRemovalReason::Killed);
}

void Enemy::Shoot(Utility::Pool<Missile>& missiles)
{
	if (Missile* missile = missiles.Acquire())
	{
		const WeaponSettings& settings = m_weapon.getSettings();

		const Utility::Vector2<float> muzzle{ m_currentPosition.x + HITBOX_WIDTH/2, m_currentPosition.y + HITBOX_HEIGHT};

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

	if (m_observer)
	{
		m_observer->OnEnemyRemoved({.reason = reason, .position = m_currentPosition});
		m_observer = nullptr;
	}

}
