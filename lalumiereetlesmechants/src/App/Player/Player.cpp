#include "Player.h"
#include <algorithm>
#include <iostream>
#include <ostream>
#include <windows.h>


Player::Player(const Utility::Vector2<float>& startPosition, Pool<Missile>& missiles) : m_position(startPosition), m_missilePool(&missiles)
{

}

void Player::Update(float dt, const InputManager& input)
{
	m_shotCountdown.Update(dt);

	Move(dt, ReadDirection(input));
	Shoot(ReadShoot(input));

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

void Player::Shoot(bool shoot)
{
	if (shoot && m_shotCountdown.IsFinished() && m_currentMissile > 0)
	{
		if (Missile* missile = m_missilePool->Acquire())
		{
			missile->Launch(m_position, {0.f, 45.f});
			m_shotCountdown.Start(m_shootCooldown);
			--m_currentMissile;
			std::cout << "Shoot, remaining bullets : " << m_currentMissile <<  std::endl;
		}
	}
}
