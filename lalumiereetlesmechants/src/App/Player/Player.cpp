#include "Player.h"
#include <windows.h>

Player::Player(const Utility::Vector2<float>& inStartPosition, Utility::Pool<Missile>& inMissiles)
	: Entity(inStartPosition), m_missilePool(&inMissiles), m_health(MAX_HEALTH),
	m_hitbox{ .offset = { HITBOX_OFFSET_X, HITBOX_OFFSET_Y }, .size = { HITBOX_WIDTH, HITBOX_HEIGHT } },
	m_body{ .size = { BODY_WIDTH, BODY_HEIGHT } }
{
	m_spriteType = SpriteUtils::SpriteType::Player;
	SetInRenderQueue(true);
}

void Player::Update(float inDeltaTime, const InputManager& inInput, const Collision::CellRect& inBounds)
{
	if (IsDead())
	{
		return;
	}

	m_fireCooldown.Update(inDeltaTime);
	m_invincibility.Update(inDeltaTime);
	SetInRenderQueue(IsVisible());

	Move(inDeltaTime, ReadDirection(inInput), inBounds);
	if (ReadShoot(inInput))
	{
		TryShoot();
	}
}

DamageResult Player::TakeDamage(const DamageInfos& inDamage)
{
	if (IsDead() || IsInvincible())
	{
		return DamageResult::Ignored;
	}

	if (m_health.TakeDamage(inDamage.amount))
	{
		Die();
		return DamageResult::Killed;
	}

	m_invincibility.Start(HIT_INVINCIBILITY_DURATION);
	return DamageResult::Hit;
}

void Player::AddXp(int inAmount)
{
	const int levelBefore = m_experience.getLevel();
	m_experience.AddXp(inAmount);

	for (int level = levelBefore + 1; level <= m_experience.getLevel(); ++level)
	{
		m_health.Heal(LEVELS[level - 1].livesRestored);
	}
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

Utility::Vector2<int> Player::ReadDirection(const InputManager& inInput)
{
	Utility::Vector2<int> direction;

	if (inInput.IsDown(VK_UP) || inInput.IsDown('Z') || inInput.IsDown('W'))
	{
		direction.y -= 1;
	}
	if (inInput.IsDown(VK_DOWN) || inInput.IsDown('S'))
	{
		direction.y += 1;
	}
	if (inInput.IsDown(VK_LEFT) || inInput.IsDown('Q') || inInput.IsDown('A'))
	{
		direction.x -= 1;
	}
	if (inInput.IsDown(VK_RIGHT) || inInput.IsDown('D'))
	{
		direction.x += 1;
	}

	return direction;
}

void Player::Move(float inDeltaTime, const Utility::Vector2<int>& inDirection, const Collision::CellRect& inBounds)
{
	const float diagonalFactor = (inDirection.x != 0 && inDirection.y != 0) ? DIAGONAL_FACTOR : 1.f;
	const float stepX = static_cast<float>(inDirection.x) * SPEED_X * diagonalFactor * inDeltaTime;
	const float stepY = static_cast<float>(inDirection.y) * SPEED_Y * diagonalFactor * inDeltaTime;

	TryMoveTo({ m_position.x + stepX, m_position.y }, inBounds);
	TryMoveTo({ m_position.x, m_position.y + stepY }, inBounds);
}

void Player::TryMoveTo(const Utility::Vector2<float>& inTarget, const Collision::CellRect& inBounds)
{
	if (inBounds.ContainsRect(m_body.At(inTarget)))
	{
		m_position = inTarget;
	}
}

bool Player::ReadShoot(const InputManager& inInput)
{
	return (inInput.IsDown(VK_SPACE) || inInput.IsDown('E'));
}

void Player::TryShoot()
{
	if (!m_fireCooldown.IsFinished())
	{
		return;
	}

	const ShotSettings& shot = LEVELS[m_experience.getLevel() - 1].shot;
	const float missileSpeed = MissileArchetypes::Get(shot.missile).speed;
	for (int i = 0; i < shot.missileCount; ++i)
	{
		if (Missile* missile = m_missilePool->Acquire())
		{
			const float fromCenter = i - (shot.missileCount - 1) / 2.f;
			const float missileX = m_position.x + BODY_WIDTH / 2 + fromCenter * shot.spacing;
			missile->Launch(shot.missile, { missileX, m_position.y - MISSILE_SPAWN_OFFSET_Y }, { fromCenter * shot.spreadSpeed, -missileSpeed });
		}
	}
	m_fireCooldown.Start(shot.fireInterval);
}

void Player::Die()
{
	SetInRenderQueue(false);
}
