#include "Player.h"
#include <windows.h>


Player::Player(const Utility::Vector2<float>& startPosition, Utility::Pool<Missile>& missiles)
	: m_position(startPosition), m_missilePool(&missiles), m_health(MAX_HEALTH),
	  m_hitbox{ .offset = { HITBOX_OFFSET_X, HITBOX_OFFSET_Y }, .size = { HITBOX_WIDTH, HITBOX_HEIGHT } },
	  m_body{ .size = { BODY_WIDTH, BODY_HEIGHT } }
{
	this->SetSpritePosition(&m_position);
	m_spriteType = SpriteType::PlayerType;
}

void Player::Update(float dt, const InputManager& input, const Collision::CellRect& bounds)
{
	if (IsDead())
	{
		return;
	}

	m_fireCooldown.Update(dt);
	m_invincibility.Update(dt);

	Move(dt, ReadDirection(input), bounds);
	if (ReadShoot(input))
	{
		TryShoot();
	}
}

DamageResult Player::TakeDamage(const DamageInfos& damage)
{
	if (IsDead() || IsInvincible())
	{
		return DamageResult::Ignored;
	}

	const bool killed = m_health.TakeDamage(damage.amount);
	m_invincibility.Start(HIT_INVINCIBILITY_DURATION);

	return killed ? DamageResult::Killed : DamageResult::Hit;
}

DamageInfos Player::getContactDamage() const
{
	return { .amount = CONTACT_DAMAGE, .type = DamageType::Collision, .source = Team::Player, .position = m_position };
}

bool Player::IsVisible() const
{
	if (!IsInvincible())
	{
		return true;
	}

	const int blinkIndex = static_cast<int>(m_invincibility.getRemaining() / BLINK_PERIOD);
	return blinkIndex % 2 == 0;
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

void Player::Move(float dt, const Utility::Vector2<int>& direction, const Collision::CellRect& bounds)
{
	const float stepX = static_cast<float>(direction.x) * m_speed.x * dt;
	const float stepY = static_cast<float>(direction.y) * m_speed.y * dt;

	TryMoveTo({ m_position.x + stepX, m_position.y }, bounds);
	TryMoveTo({ m_position.x, m_position.y + stepY }, bounds);
}

void Player::TryMoveTo(const Utility::Vector2<float>& target, const Collision::CellRect& bounds)
{
	if (bounds.Contains(m_body.At(target)))
	{
		m_position = target;
	}
}

bool Player::ReadShoot(const InputManager& input)
{
	return (input.IsDown(VK_SPACE) || input.IsDown('E'));
}

void Player::TryShoot()
{
	if (!m_fireCooldown.IsFinished())
	{
		return;
	}

	if (Missile* missile = m_missilePool->Acquire())
	{
		missile->Launch({ m_position.x + BODY_WIDTH / 2, m_position.y - 1.f }, { 0.f, -MISSILE_SPEED }, Team::Player, MISSILE_DAMAGE);
		m_fireCooldown.Start(m_fireDelay);
	}
}
