#include "Player.h"

void Player::move(Utility::Vector2<int> desiredPosition)
{
	if (desiredPosition != m_currentPosition)
	{
		m_currentPosition = desiredPosition;
	}
}

void Player::shoot()
{
	//TODO : Instancier les missiles
}
