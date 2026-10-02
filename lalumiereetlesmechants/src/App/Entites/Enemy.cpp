#include "Enemy.h"

#include <algorithm>

Enemy::Enemy() : Entity()
{
}

Enemy::Enemy(const Utility::Vector2<int>& position, Sprite* sprite) : Entity(position, sprite)
{
}

Enemy::~Enemy()
{

}

void Enemy::Update()
{

}

void Enemy::TakeDamage(const int damages)
{
	if (m_isDead || damages <= 0)
	{
		return;
	}

	m_health -= damages;
	m_health = std::max(m_health, 0);
	if (m_health <= 0)
	{
		Die();
	}
}

void Enemy::Die()
{
	m_isDead = true;
}

