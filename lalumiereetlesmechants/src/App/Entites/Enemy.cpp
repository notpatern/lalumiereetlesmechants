#include "Enemy.h"

Enemy::Enemy() : m_health(MAX_HEALTH), m_hitbox{ .size = { HITBOX_WIDTH, HITBOX_HEIGHT } }, m_observer(nullptr), m_isActive(false)
{
}

void Enemy::Spawn(const Utility::Vector2<float>& position, const Utility::Vector2<float>& velocity, IEnemyObserver* observer)
{
	m_currentPosition = position;
	m_velocity = velocity;
	m_health.Reset();
	m_hitFlash.Start(0.f);
	m_isActive = true;
	m_observer = observer;
}

void Enemy::Update(float dt)
{
	if (!m_isActive)
	{
		return;
	}

	m_currentPosition.x += m_velocity.x * dt;
	m_currentPosition.y += m_velocity.y * dt;

	m_hitFlash.Update(dt);
}

void Enemy::Deactivate()
{
	RemoveFromPlay(EnemyRemovalReason::Escaped);
}

void Enemy::TakeDamage(const DamageInfos& damage)
{
	if (!m_isActive)
	{
		return;
	}

	if (damage.type == DamageType::Collision)
	{
		Die();
		return;
	}

	if (m_health.TakeDamage(damage.amount))
	{
		Die();
	}
	else
	{
		m_hitFlash.Start(HIT_FLASH_DURATION);
	}
}

DamageInfos Enemy::getContactDamage() const
{
	return { .amount = CONTACT_DAMAGE, .type = DamageType::Collision, .source = Team::Enemy, .position = m_currentPosition };
}

void Enemy::Die()
{
	RemoveFromPlay(EnemyRemovalReason::Killed);
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
