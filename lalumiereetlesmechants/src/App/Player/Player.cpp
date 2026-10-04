#include "Player.h"
#include <windows.h>


Player::Player(const Utility::Vector2<float>& startPosition, Utility::Pool<Missile>& missiles)
	: m_position(startPosition), m_missilePool(&missiles), m_health(MAX_HEALTH), m_hitbox{ .size = { HITBOX_WIDTH, HITBOX_HEIGHT } }
{

}

void Player::Update(float dt, const InputManager& input)
{
	if (IsDead())
	{
		return;
	}

	m_fireCooldown.Update(dt);
	m_invincibility.Update(dt);

	Move(dt, ReadDirection(input));
	if (ReadShoot(input))
	{
		TryShoot();
	}
}

void Player::TakeDamage(const DamageInfos& damage)
{
	if (IsDead() || IsInvincible())
	{
		return;
	}

	m_health.TakeDamage(damage.amount);
	m_invincibility.Start(HIT_INVINCIBILITY_DURATION);
}

DamageInfos Player::getContactDamage() const
{
	return { .amount = CONTACT_DAMAGE, .type = DamageType::Collision, .source = Team::Player, .position = m_position };
}

Utility::Vector2<int> Player::ReadDirection(const InputManager& input)
{
	Utility::Vector2<int> direction;

	if (input.IsDown(VK_UP) || input.IsDown('Z') || input.IsDown('W'))
	{
		direction.y -= 1;
	}
	if (input.IsDown(VK_DOWN) || input.IsDown('S'))
	{
		direction.y += 1;
	}
	if (input.IsDown(VK_LEFT) || input.IsDown('Q') || input.IsDown('A'))
	{
		direction.x -= 1;
	}
	if (input.IsDown(VK_RIGHT) || input.IsDown('D'))
	{
		direction.x += 1;
	}

	return direction;
}

void Player::Move(float dt, const Utility::Vector2<int>& direction)
{
	m_position.x += static_cast<float>(direction.x) * m_speed.x * dt;
	m_position.y += static_cast<float>(direction.y) * m_speed.y * dt;
}

bool Player::ReadShoot(const InputManager& input)
{
	return (input.IsDown(VK_SPACE) || input.IsDown('E'));
}

void Player::TryShoot()
{
	if (!m_fireCooldown.IsFinished() || m_remainingMissiles <= 0)
	{
		return;
	}

	if (Missile* missile = m_missilePool->Acquire())
	{
		missile->Launch({ m_position.x, m_position.y - 1.f }, { 0.f, -MISSILE_SPEED }, Team::Player, MISSILE_DAMAGE);
		m_fireCooldown.Start(m_fireDelay);
		--m_remainingMissiles;
	}
}
