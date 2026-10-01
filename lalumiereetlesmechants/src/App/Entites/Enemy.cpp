#include "Enemy.h"

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
