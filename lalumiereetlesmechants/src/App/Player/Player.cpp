#include "Player.h"
#include <windows.h>


Player::Player(const Utility::Vector2<float>& startPosition) : m_position(startPosition)
{
}

void Player::Update(float dt, const InputManager& input)
{
	Move(dt, ReadDirection(input));
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
	m_position.x += direction.x * m_speed.x * dt;
	m_position.y += direction.y * m_speed.y * dt;
}


